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

			ScopeNode* m_Parent;
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

		inline static float FrameTime() { return s_PreviousFrameResults ? s_PreviousFrameResults->DurationMs() : 0.0f; }

		static void InOrder(
			std::function<bool(ScopeNode* node, unsigned int recursionLevel, unsigned int id, unsigned int parentId)> continueRecursionHandler, 
			std::function<void(ScopeNode* node, unsigned int recursionLevel, unsigned int id, unsigned int parentId)> afterRecursionHandler
		);

	private:
		static void InOrderInner(
			std::function<bool(ScopeNode* node, unsigned int recursionLevel, unsigned int id, unsigned int parentId)> continueRecursionHandler, 
			std::function<void(ScopeNode* node, unsigned int recursionLevel, unsigned int id, unsigned int parentId)> afterRecursionHandler, 
			ScopeNode* node, 
			unsigned int recursionLevel,
			unsigned int selfId
		);

		inline static bool s_CapturingFrame = false;
		inline static ScopeNode* s_ActiveProfilerNode = nullptr;
		inline static ScopeNode* s_PreviousFrameResults = nullptr;
	};
}
