#ifndef WEBDRIVERXX_WINDOW_H
#define WEBDRIVERXX_WINDOW_H

#include "types.h"
#include "conversions.h"
#include "detail/resource.h"
#include <string>

namespace webdriverxx {

class Window { // copyable
public:
	Window(const std::string& handle, const detail::Shared<detail::Resource>& resource)
		: handle_(handle)
		, resource_(resource)
	{}

	~Window() {
		delete this;
	}

	void close() const {
		this->~Window();
	}

	std::string GetHandle() const {
		return handle_;
	}

	Size GetSize() const {
		return resource_->GetValue<Size>("size");
	}

	const Window& SetSize(const Size& size) const {
		resource_->PostValue("size", size);
		return *this;
	}

	Point GetPosition() const {
		return resource_->GetValue<Point>("position");
	}

	const Window& SetPosition(const Point& position) const {
		resource_->PostValue("position", position);
		return *this;
	}

	const Window& Maximize() const {
		resource_->Post("maximize");
		return *this;
	}

	const Window& Minimize() const {
		resource_->Post("minimize");
		return *this;
	}

	const Window& Fullscreen() const {
		resource_->Post("fullscreen");
		return *this;
	}

private:
	std::string handle_;
	detail::Shared<detail::Resource> resource_;
};

} // namespace webdriverxx

#endif
