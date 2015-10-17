#pragma once

#include <vector>
#include <functional>
#include <mutex>

#include "Hooker.h"

namespace GWAPI{

	// Shoutouts to DarthTon @ unknowncheats.me for this class.

	class GameThreadMgr{
	public:

		// For use only in gameloop hook.
		void __stdcall CallFunctions();

		// Add function to gameloop queue, only use if you know what you're doing.
		template<typename F, typename... ArgTypes>
		void Enqueue(F&& Func, ArgTypes&&... Args)
		{
			std::unique_lock<std::mutex> VecLock(call_vector_mutex_);
			calls_.emplace_back(std::bind(std::forward<F>(Func), std::forward<ArgTypes>(Args)...));
		}
		template<typename F, typename... ArgTypes>
		void AddPermanentCall(F&& Func, ArgTypes&&... Args)
		{
			std::unique_lock<std::mutex> VecLock(call_vector_mutex_);
			calls_permanent_.emplace_back(std::bind(std::forward<F>(Func), std::forward<ArgTypes>(Args)...));
		}


		static void gameLoopHook();
		static void renderHook();
		void ToggleRenderHook();

	private:

		friend class GWAPIMgr;

		void RestoreHooks();
		GameThreadMgr(GWAPIMgr* obj);
		~GameThreadMgr();

		std::vector<std::function<void(void)> > calls_;
		std::vector<std::function<void(void)> > calls_permanent_;
		mutable std::mutex call_vector_mutex_;
		bool render_state_;
		Hook hk_game_thread_;
		GWAPIMgr* const parent_;
	};

}

