#ifndef SHARED_QUEUE_HPP
#define SHARED_QUEUE_HPP

#include <condition_variable>
#include <mutex>
#include <queue>

template <typename T> class SharedQueue {
  private:
	std::queue<T> queue;
	std::mutex mutex;
	std::condition_variable cond;

  public:
	SharedQueue() = default;
	SharedQueue(const SharedQueue&) = delete;			 // disable copying
	SharedQueue& operator=(const SharedQueue&) = delete; // disable assignment

	SharedQueue(SharedQueue&& other) {
		std::lock_guard<std::mutex> lock(mutex);
		queue = std::move(other.queue);
	}

	virtual ~SharedQueue() = default;

	void push(T item) {
		std::lock_guard<std::mutex> lock(mutex);
		queue.push(item);
		cond.notify_one();
	}

	T pop() {
		std::unique_lock<std::mutex> lock(mutex);
		cond.wait(lock, [this] { return !queue.empty(); });
		T item = queue.front();
		queue.pop();
		return item;
	}

	bool empty() {
		std::lock_guard<std::mutex> lock(mutex);
		return queue.empty();
	}

	int size() {
		std::lock_guard<std::mutex> lock(mutex);
		return queue.size();
	}

	void clear() {
		std::lock_guard<std::mutex> lock(mutex);
		while (!queue.empty()) {
			queue.pop();
		}
	}

	void wait() {
		std::unique_lock<std::mutex> lock(mutex);
		cond.wait(lock, [this] { return !queue.empty(); });
	}

	void notify() {
		std::lock_guard<std::mutex> lock(mutex);
		cond.notify_one();
	}

	void notify_all() {
		std::lock_guard<std::mutex> lock(mutex);
		cond.notify_all();
	}
};
#endif
