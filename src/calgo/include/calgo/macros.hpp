#include <functional>

#define CA_CALLBACK(NAME, ...) \
	public: \
		void NAME##Callback(const std::function<void(__VA_ARGS__)>& callback) { \
			p_##NAME##Callback = callback; \
		} \
		const std::function<void(__VA_ARGS__)>& NAME##Callback() const { \
			return p_##NAME##Callback; \
		} \
		std::function<void(__VA_ARGS__)>& NAME##Callback() { \
			return p_##NAME##Callback; \
		} \
	protected: \
		std::function<void(__VA_ARGS__)> p_##NAME##Callback; \
	public:
