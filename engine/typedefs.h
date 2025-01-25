#pragma once

#include <complex>
#include <string>
#include <string_view>
#include <memory>
#include <type_traits>
#include <vector>
#include <functional>
#include "macro.h"

#define WRAP_TYPE_RIZZ(x) RIZZ::##x

namespace RIZZ {

	template <typename T>
	constexpr T ABS(T m_v) {
		return m_v < 0 ? -m_v : m_v;
	}

	template <typename T>
	constexpr const T SIGN(const T m_v) {
		return m_v > 0 ? +1.0f : (m_v < 0 ? -1.0f : 0.0f);
	}

	template <typename T, typename T2>
	constexpr auto MIN(const T m_a, const T2 m_b) {
		return m_a < m_b ? m_a : m_b;
	}

	template <typename T, typename T2>
	constexpr auto MAX(const T m_a, const T2 m_b) {
		return m_a > m_b ? m_a : m_b;
	}

	template <typename T, typename T2, typename T3>
	constexpr auto CLAMP(const T m_a, const T2 m_min, const T3 m_max) {
		return m_a < m_min ? m_min : (m_a > m_max ? m_max : m_a);
	}

	template <typename T>
	struct Comparator {
		INLINE bool operator()(const T& p_a, const T& p_b) const { return (p_a < p_b); }
	};


	class Point {
	private:
		double m_X, m_Y;
	public:
		Point() NOEXCEPT : m_X(0), m_Y(0) {}
		Point(double x, double y) NOEXCEPT : m_X(x), m_Y(y)  {}

		double GetX() const { return m_X; }
		double GetY() const { return m_Y; }

		template<typename T>
		T TryCastX() const {
			static_assert(std::is_integral_v<T>, "Conversion error: T must be an integral type");
			return static_cast<T>(m_X);
		}

		template<typename T>
		T TryCastY() const {
			static_assert(std::is_integral_v<T>, "Conversion error: T must be an integral type");
			return static_cast<T>(m_Y);
		}


		Point& operator=(const Point& other) {
			if (this != &other) {
				m_X = other.m_X;
				m_Y = other.m_Y;
			}
			return *this;
		}

		bool operator==(const Point& other) const {
			return m_X == other.m_X && m_Y == other.m_Y;
		}

		bool operator!=(const Point& other) const {
			return !(*this == other);
		}

		double distanceTo(const Point& other) const {
			return std::sqrt(std::pow(m_X - other.m_X, 2) + std::pow(m_Y - other.m_Y, 2));
		}

		bool isEmpty() const { return m_X == 0 || m_Y == 0; }
	};


	class Size {
	private:
		int m_W, m_H;

	public:
		Size() NOEXCEPT : m_W(0), m_H(0) {}
		Size(int w, int h) NOEXCEPT : m_W(w), m_H(h) {}

		int Width() const { return m_W; }
		int Height() const { return m_H; }

		Size& operator=(const Size& other) {
			if (this != &other) {
				m_W = other.m_W;
				m_H = other.m_H;
			}
			return *this;
		}

		bool operator==(const Size& other) const {
			return m_W == other.m_W && m_H == other.m_H;
		}

		bool operator!=(const Size& other) const {
			return !(*this == other);
		}

		bool isEmpty() const { return m_W == 0 || m_H == 0; }
	};


	using String = std::string;
	using StringView = std::string_view;

	template<typename T1, typename T2>
	using Pair = std::pair<T1, T2>;

	template<typename T>
	using Pointer = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Pointer<T> CreatePointer(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Reference = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Reference<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Optional = std::optional<T>;


	template<typename TObject = void>
	class Object {
	private:
		TObject Value;  

	public:
		using Type = TObject;
		using Pointer = TObject*;
		using Reference = TObject&;

		constexpr Object() = default;
		constexpr explicit Object(const TObject& value) : Value(value) {}
		constexpr explicit Object(TObject&& value) noexcept : Value(std::move(value)) {}

		constexpr const TObject& Get() const noexcept { return Value; }
		constexpr TObject& GetMutable() noexcept { return Value; }

		constexpr operator const TObject& () const noexcept { return Value; }
		constexpr operator TObject& () noexcept { return Value; }

		constexpr Object& operator=(const TObject& value) noexcept {
			Value = value;
			return *this;
		}

		constexpr Object& operator=(TObject&& value) noexcept {
			Value = std::move(value);
			return *this;
		}

		constexpr bool operator==(const Object& other) const noexcept { return Value == other.Value; }
		constexpr bool operator!=(const Object& other) const noexcept { return !(*this == other); }

		template<typename T>
		T* to_cast() {
			return dynamic_cast<T*>(&Value); 
		}
	};


} // namespace RIZZ
