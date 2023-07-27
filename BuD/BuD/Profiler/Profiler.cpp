#include "bud_pch.h"
#include "Profiler.h"

namespace BuD
{
	void Profiler::BeginScope(std::string name)
	{
		auto profilerNode = std::make_shared<ScopeNode>(name);

		if (s_ActiveProfilerNode)
		{
			profilerNode->m_Parent = s_ActiveProfilerNode;
			s_ActiveProfilerNode->m_Children.push_back(profilerNode);
		}

		s_ActiveProfilerNode = profilerNode;
	}

	void Profiler::EndScope()
	{
		if (!s_ActiveProfilerNode || (!s_ActiveProfilerNode->m_Parent && s_CapturingFrame))
		{
			Log::WriteError(L"Profiler error, EndScope used without BeginScope");
			throw std::runtime_error("");
		}

		s_ActiveProfilerNode->EndScope();

		if (s_ActiveProfilerNode->m_Parent)
		{
			s_ActiveProfilerNode = s_ActiveProfilerNode->m_Parent;

			return;
		}

		s_PreviousFrameResults = s_ActiveProfilerNode;
		s_ActiveProfilerNode = nullptr;
	}

	void Profiler::BeginFrame()
	{
		s_CapturingFrame = true;
		s_ActiveProfilerNode = nullptr;

		BeginScope("Frame start");
	}

	void Profiler::EndFrame()
	{
		s_CapturingFrame = false;

		EndScope();

		if (s_ActiveProfilerNode)
		{
			Log::WriteError(L"Profiler error, unmatching BeginScope/EndScope in code");
			throw std::runtime_error("Profiler error, unmatching BeginScope/EndScope in code");
		}
	}

	void Profiler::InOrder(
		std::function<bool(std::shared_ptr<ScopeNode> node, unsigned int recursionLevel, unsigned int childId, unsigned int parentId)> continueRecursionHandler, 
		std::function<void(std::shared_ptr<ScopeNode> node, unsigned int recursionLevel, unsigned int childId, unsigned int parentId)> afterRecursionHandler
	)
	{
		if (!s_PreviousFrameResults)
		{
			return;
		}

		if (continueRecursionHandler(s_PreviousFrameResults, 0, 0, -1))
		{
			InOrderInner(continueRecursionHandler, afterRecursionHandler, s_PreviousFrameResults, 1, 0);

			afterRecursionHandler(s_PreviousFrameResults, 0, 0, -1);
		}
	}

	void Profiler::InOrderInner(
		std::function<bool(std::shared_ptr<ScopeNode> node, unsigned int recursionLevel, unsigned int childId, unsigned int parentId)> continueRecursionHandler, 
		std::function<void(std::shared_ptr<ScopeNode> node, unsigned int recursionLevel, unsigned int childId, unsigned int parentId)> afterRecursionHandler, 
		std::shared_ptr<ScopeNode> node, 
		unsigned int recursionLevel,
		unsigned int selfId
	)
	{
		unsigned int childId = 0;

		for (auto& child : node->m_Children)
		{
			if (continueRecursionHandler(child, recursionLevel, childId, selfId))
			{
				InOrderInner(continueRecursionHandler, afterRecursionHandler, child, recursionLevel + 1, childId);

				afterRecursionHandler(child, recursionLevel, childId, selfId);
			}

			childId++;
		}
	}

	Profiler::ScopeNode::ScopeNode(std::string name)
		: m_Name(name), m_ScopeEnded(false), m_StartTime(Clock::Now()), m_EndTime(Clock::Now())
	{
	}

	void Profiler::ScopeNode::EndScope()
	{
		m_EndTime = Clock::Now();
		m_ScopeEnded = true;
	}
}
