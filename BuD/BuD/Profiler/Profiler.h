#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <Utils/Timepoint.h>

namespace BuD
{
	class Profiler
	{
	public:
		Profiler() = delete;
		Profiler(const Profiler&) = delete;
		Profiler(Profiler&&) = delete;

		struct ScopeNode
		{
		public:
			explicit ScopeNode(std::string name);

			std::string m_Name;

			std::shared_ptr<ScopeNode> m_Parent;
			std::vector<std::shared_ptr<ScopeNode>> m_Children;

			float Duration() const { return m_ScopeEnded ? (float)(m_EndTime - m_StartTime) : 0.0f; }
			float DurationMs() const { return 1000.0f * Duration(); }

		private:
			void EndScope();

			Timepoint m_StartTime;
			Timepoint m_EndTime;
			bool m_ScopeEnded;

			friend class Profiler;
		};

		static void BeginScope(std::string name);
		static void EndScope();

		static void BeginFrame();
		static void EndFrame();

		static void InOrder(
			std::function<bool(std::shared_ptr<ScopeNode> node, unsigned int recursionLevel, unsigned int id, unsigned int parentId)> continueRecursionHandler, 
			std::function<void(std::shared_ptr<ScopeNode> node, unsigned int recursionLevel, unsigned int id, unsigned int parentId)> afterRecursionHandler
		);

	private:
		static void InOrderInner(
			std::function<bool(std::shared_ptr<ScopeNode> node, unsigned int recursionLevel, unsigned int id, unsigned int parentId)> continueRecursionHandler, 
			std::function<void(std::shared_ptr<ScopeNode> node, unsigned int recursionLevel, unsigned int id, unsigned int parentId)> afterRecursionHandler, 
			std::shared_ptr<ScopeNode> node, 
			unsigned int recursionLevel,
			unsigned int selfId
		);

		inline static bool s_CapturingFrame = false;
		inline static std::shared_ptr<ScopeNode> s_ActiveProfilerNode = nullptr;
		inline static std::shared_ptr<ScopeNode> s_PreviousFrameResults = nullptr;
	};
}
