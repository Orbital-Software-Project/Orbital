#pragma once

#include <vector>
#include <memory>
#include <chrono>
#include <thread>
#include <atomic>
#include <mutex>

#include "Editor/Base/Percentage.hpp"

namespace Orb {


    class ITask {
    public:
        enum class TaskStatus {
            Created,
            Running,
            Canceled,
            Finished,
            Paused,
            Errored
        };

        struct ITaskReport {
            Percentage Percentage;
        };

        virtual ~ITask() {}

        virtual ITaskReport* GetReport() = 0;

        virtual void Run() = 0;

        virtual TaskStatus GetStatus() {
            return this->Status;
        }

        virtual void Cancel() {
            this->CancelRequested = true;
        }

        virtual void Pause() {
            this->PauseRequested = true;
        }

        virtual void Resume() {
            this->PauseRequested = false;
        }

    protected:
        std::atomic<TaskStatus> Status = TaskStatus::Created;
        std::atomic<bool> CancelRequested = false;
        std::atomic<bool> PauseRequested = false;

        std::mutex TaskReportMutex;


        void PauseLoop() {
            while (this->PauseRequested && !this->CancelRequested) {
                std::this_thread::sleep_for(std::chrono::seconds(3)); // wait for 3 seconds
            }
        }

    };


    class TaskWorker {

    public:
        static TaskWorker& GetInstance()
        {
            static TaskWorker instance;
            return instance;
        }

        ~TaskWorker() {
            this->running = false;

            this->CancelAllTasks();

            this->taskWorker.join();


        }

        TaskWorker(TaskWorker const&) = delete;

        void operator=(TaskWorker const&) = delete;

        void Exec(std::shared_ptr<ITask> task, bool sync = false) {
            if (sync) {
                task->Run();
            }
            else {
                this->task_collection.push_back(task);
            }
        }

        std::vector<std::shared_ptr<ITask>> GetFinishedTasks() {
            return this->finished_tasks;
        }

        std::vector<std::shared_ptr<ITask>> GetRunningTasks() {
            return this->task_collection;
        }

        void CancelAllTasks() {
            for (auto task : this->GetRunningTasks()) {
                task->Cancel();
            }
        }

    private:
        TaskWorker() {
            this->taskWorker = std::thread(&TaskWorker::doTasks, this);
        }

        void doTasks() {
            // Execute and finish tasks one by one
            while (this->running) {
                if (this->task_collection.size() > 0) {

                    std::shared_ptr<ITask> task = this->task_collection.back(); // get last

                    // Only execute newly created tasks
                    if (task->GetStatus() == ITask::TaskStatus::Created) {

                        task->Run();

                        this->finished_tasks.push_back(task);

                        this->task_collection.pop_back(); // remove last
                    }
                }
                else {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
            }
        }

    private:
        std::thread taskWorker;
        std::vector<std::shared_ptr<ITask>> task_collection;

        std::vector< std::shared_ptr<ITask>> finished_tasks;
        std::atomic<bool> running = true;


    };


}