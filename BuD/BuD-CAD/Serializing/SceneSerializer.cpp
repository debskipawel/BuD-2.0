#include "SceneSerializer.h"

#include <Serializer.h>

#include <Visitors/Serialization/SerializationVisitor.h>

SceneSerializer::SceneSerializer(MainDataLayer& dataLayer)
    : m_MainDataLayer(dataLayer)
{
}

bool SceneSerializer::Save()
{
    const auto& filepath = m_MainDataLayer.m_SceneDataLayer.m_PathToFile;

    if (filepath.empty())
    {
        return false;
    }

    auto& scene = MG1::Scene::Get();
    scene.Clear();

    auto camera = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.m_Scene.ActiveCamera();
    auto viewMatrix = camera->ViewMatrix();
    
    dxm::Vector3 position, scale;
    dxm::Quaternion rotation;
    
    if (viewMatrix.Decompose(position, rotation, scale))
    {
        auto eulerAngles = rotation.ToEuler();
        auto focusPoint = camera->EyePosition() + camera->Front();

        auto serializedCamera = MG1::Camera();
        
        serializedCamera.distance = 1.0f;
        serializedCamera.focusPoint = { focusPoint.x, focusPoint.y, focusPoint.z };
        serializedCamera.rotation = { DirectX::XMConvertToDegrees(eulerAngles.x), DirectX::XMConvertToDegrees(eulerAngles.y) };

        scene.camera = serializedCamera;
    }

    std::unique_ptr<AbstractVisitor> serializationVisitor = std::make_unique<SerializationVisitor>();

    for (auto& [id, object] : m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.m_ObjectList)
    {
        serializationVisitor->Visit(object);
    }

    auto serializer = MG1::SceneSerializer();
    
    try
    {
        serializer.SaveScene(filepath);
    }
    catch (MG1::SerializerException e)
    {
        auto filename = filepath.filename();
        auto message = std::format("Error while saving scene to {}: {}", filename.string(), e.what());
        
        BuD::Log::WriteWarning(message);

        return false;
    }

    return true;
}

bool SceneSerializer::Load()
{
    const auto& filepath = m_MainDataLayer.m_SceneDataLayer.m_PathToFile;

    if (filepath.empty())
    {
        return false;
    }

    auto serializer = MG1::SceneSerializer();

    try
    {
        serializer.LoadScene(filepath);
    }
    catch (MG1::SerializerException e)
    {
        auto filename = filepath.filename();
        auto message = std::format("Error while loading scene from {}: {}", filename.string(), e.what());

        BuD::Log::WriteError(message);

        return false;
    }

    ClearCurrentScene();
    ConstructLoadedScene();

    return true;
}

void SceneSerializer::ClearCurrentScene()
{
    m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.Clear();
    m_MainDataLayer.m_SceneDataLayer.m_ManuallySelected.Clear();
    m_MainDataLayer.m_SceneDataLayer.m_SelectedForTransform.Clear();
}

void SceneSerializer::ConstructLoadedScene()
{
    auto& sceneCAD = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD;
    auto& scene = MG1::Scene::Get();

    if (scene.camera.has_value())
    {
        auto& serializedCamera = scene.camera.value();
        auto camera = sceneCAD.m_Scene.ActiveCamera();
        auto focusPoint = dxm::Vector3(serializedCamera.focusPoint.x, serializedCamera.focusPoint.y, serializedCamera.focusPoint.z);

        auto rotation = dxm::Vector3(
            DirectX::XMConvertToRadians(serializedCamera.rotation.x),
            DirectX::XMConvertToRadians(serializedCamera.rotation.y),
            0.0f
        );

        auto position = focusPoint + dxm::Vector3::Transform(serializedCamera.distance * dxm::Vector3::UnitZ, dxm::Matrix::CreateFromYawPitchRoll(rotation));

        // TODO: take camera rotation into consideration
        camera->MoveTo(position);
    }

    std::unordered_map<uint32_t, std::weak_ptr<Point>> pointMap;

    for (auto& point : scene.points)
    {
        dxm::Vector3 position = { point.position.x, point.position.y, point.position.z };

        auto pointObject = std::make_shared<Point>(sceneCAD.m_Scene, position);

        pointObject->m_Tag = point.name;

        sceneCAD.m_ObjectList.emplace(pointObject->Id(), pointObject);
        pointMap.emplace(point.GetId(), pointObject);
    }

    for (auto& torus : scene.tori)
    {
        auto transform = TransformComponent();
        transform.m_Position = { torus.position.x, torus.position.y, torus.position.z };
        transform.m_Rotation = { torus.rotation.x, torus.rotation.y, torus.rotation.z };
        transform.m_Scale = { torus.scale.x, torus.scale.y, torus.scale.z };

        auto torusObject = std::make_shared<Torus>(sceneCAD.m_Scene, transform.m_Position, torus.largeRadius, torus.smallRadius, torus.samples.x, torus.samples.y);

        torusObject->m_Tag = torus.name;
        torusObject->m_Transform = transform;

        dxm::Vector3 rotationRadian =
        {
            DirectX::XMConvertToRadians(transform.m_Rotation.x),
            DirectX::XMConvertToRadians(transform.m_Rotation.y),
            DirectX::XMConvertToRadians(transform.m_Rotation.z)
        };

        torusObject->m_InstanceData.m_ModelMatrix = dxm::Matrix::CreateScale(transform.m_Scale) * dxm::Matrix::CreateFromYawPitchRoll(rotationRadian) * dxm::Matrix::CreateTranslation(transform.m_Position);
    
        sceneCAD.m_ObjectList.emplace(torusObject->Id(), torusObject);
    }

    for (auto& bezierCurveC0 : scene.bezierC0)
    {
        std::vector<std::weak_ptr<Point>> controlPoints;

        for (auto& controlPoint : bezierCurveC0.controlPoints)
        {
            auto& point = pointMap.at(controlPoint.GetId());

            controlPoints.push_back(point);
        }

        auto curve = std::make_shared<BezierCurveC0>(sceneCAD.m_Scene, controlPoints);

        for (auto& controlPoint : controlPoints)
        {
            auto pointShared = controlPoint.lock();
            pointShared->m_PointBasedObjects.push_back(curve);
        }

        curve->m_Tag = bezierCurveC0.name;

        sceneCAD.m_ObjectList.emplace(curve->Id(), curve);
    }

    for (auto& bezierCurveC2 : scene.bezierC2)
    {
        std::vector<std::weak_ptr<Point>> controlPoints;

        for (auto& controlPoint : bezierCurveC2.controlPoints)
        {
            auto& point = pointMap.at(controlPoint.GetId());

            controlPoints.push_back(point);
        }

        auto curve = std::make_shared<BezierCurveC2>(sceneCAD.m_Scene, controlPoints);

        for (auto& controlPoint : controlPoints)
        {
            auto pointShared = controlPoint.lock();
            pointShared->m_PointBasedObjects.push_back(curve);
        }

        curve->m_Tag = bezierCurveC2.name;

        sceneCAD.m_ObjectList.emplace(curve->Id(), curve);
    }

    for (auto& interpolatedCurve : scene.interpolatedC2)
    {
        std::vector<std::weak_ptr<Point>> controlPoints;

        for (auto& controlPoint : interpolatedCurve.controlPoints)
        {
            auto& point = pointMap.at(controlPoint.GetId());

            controlPoints.push_back(point);
        }

        auto curve = std::make_shared<YukselInterpolatingCurveC2>(sceneCAD.m_Scene, controlPoints);

        for (auto& controlPoint : controlPoints)
        {
            auto pointShared = controlPoint.lock();
            pointShared->m_PointBasedObjects.push_back(curve);
        }

        curve->m_Tag = interpolatedCurve.name;

        sceneCAD.m_ObjectList.emplace(curve->Id(), curve);
    }

    for (auto& surfaceC0 : scene.surfacesC0)
    {
        std::vector<std::shared_ptr<BaseBezierPatch>> surfacePatches;

        auto count = 0;

        for (auto& patch : surfaceC0.patches)
        {
            auto u = count % surfaceC0.size.x;
            auto v = count / surfaceC0.size.x;

            count++;

            std::vector<std::weak_ptr<Point>> controlPoints;

            for (auto& controlPoint : patch.controlPoints)
            {
                auto& point = pointMap.at(controlPoint.GetId());

                controlPoints.push_back(point);
            }

            auto rangeU = dxm::Vector2{ static_cast<float>(u) / surfaceC0.size.x, static_cast<float>(u + 1) / surfaceC0.size.x };
            auto rangeV = dxm::Vector2{ static_cast<float>(v) / surfaceC0.size.y, static_cast<float>(v + 1) / surfaceC0.size.y };

            auto patchObject = std::make_shared<BezierPatchC0>(sceneCAD.m_Scene, controlPoints, rangeU, rangeV);
            
            for (auto& controlPoint : controlPoints)
            {
                auto pointShared = controlPoint.lock();
                pointShared->m_PointBasedObjects.push_back(patchObject);
            }
            
            patchObject->m_Tag = patch.name;

            surfacePatches.push_back(patchObject);
        }

        auto surfaceObject = std::make_shared<BezierSurfaceC0>(sceneCAD, surfacePatches, surfaceC0.size.x, surfaceC0.size.y, surfaceC0.uWrapped);
        surfaceObject->UpdateOwnership();
        surfaceObject->m_Tag = surfaceC0.name;

        sceneCAD.m_ObjectList.emplace(surfaceObject->Id(), surfaceObject);
    }

    for (auto& surfaceC2 : scene.surfacesC2)
    {
        std::vector<std::shared_ptr<BaseBezierPatch>> surfacePatches;

        auto count = 0;

        for (auto& patch : surfaceC2.patches)
        {
            auto u = count % surfaceC2.size.x;
            auto v = count / surfaceC2.size.x;

            count++;

            std::vector<std::weak_ptr<Point>> controlPoints;

            for (auto& controlPoint : patch.controlPoints)
            {
                auto& point = pointMap.at(controlPoint.GetId());

                controlPoints.push_back(point);
            }

            auto rangeU = dxm::Vector2{ static_cast<float>(u) / surfaceC2.size.x, static_cast<float>(u + 1) / surfaceC2.size.x };
            auto rangeV = dxm::Vector2{ static_cast<float>(v) / surfaceC2.size.y, static_cast<float>(v + 1) / surfaceC2.size.y };

            auto patchObject = std::make_shared<BezierPatchC2>(sceneCAD.m_Scene, controlPoints, rangeU, rangeV);

            for (auto& controlPoint : controlPoints)
            {
                auto pointShared = controlPoint.lock();
                pointShared->m_PointBasedObjects.push_back(patchObject);
            }

            patchObject->m_Tag = patch.name;

            surfacePatches.push_back(patchObject);
        }

        auto surfaceObject = std::make_shared<BezierSurfaceC2>(sceneCAD, surfacePatches, surfaceC2.size.x, surfaceC2.size.y, surfaceC2.uWrapped);
        surfaceObject->UpdateOwnership();

        surfaceObject->m_Tag = surfaceC2.name;

        sceneCAD.m_ObjectList.emplace(surfaceObject->Id(), surfaceObject);
    }
}
