#pragma once
#include "async_base.hpp"
#include <memory>

namespace Os {
template <class Function, class Result_t, class... Args>
class Async : public AsyncBase<Function, Result_t, Args...> {
public:
	Async(Launch policy, Function &&f, Args &&... args) :
	base_t(policy, forward<Function>(f), forward<Args>(args)...){ }

	virtual Result_t get() override {
		base_t::wait();
		if (base_t::exceptionThrown) {
			throw *base_t::exceptionThrown;
		}
		return result;
	}

protected:
	virtual void onFuncInvoking() override {
		result = base_t::func();
	}

  private:
	using base_t = AsyncBase<Function, Result_t, Args...>;
	Result_t result;
};

template <class Function, class... Args>
class Async<Function, void, Args...> : public AsyncBase<Function, void, Args...> {
  public:
	Async(Launch policy, Function &&f, Args &&... args) :
	base_t(policy, forward<Function>(f), forward<Args>(args)...){ }

	virtual void get() override {
		base_t::wait();
		if (base_t::exceptionThrown) {
			throw *base_t::exceptionThrown;
		}
	}
	
protected:
  virtual void onFuncInvoking() override {
	  base_t::func();
	}
	
private:
  using base_t = AsyncBase<Function, void, Args...>;
};
	
template<typename Function, typename... Args>
auto async(Launch policy, Function f, Args&&... args) {
	return make_shared<Async<Function, invoke_result_t<decay_t<Function>, decay_t<Args>...>, Args...>>(policy, forward<Function>(f), forward<Args>(args)...);
}
	
template<typename Function, typename... Args>
auto async(Function f, Args&&... args) {
	return make_shared<Async<Function, invoke_result_t<decay_t<Function>, decay_t<Args>...>, Args...>>(Launch::Async, forward<Function>(f), forward<Args>(args)...);
}

}