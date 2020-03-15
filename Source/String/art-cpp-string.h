#pragma once
#include <string>

namespace art
{
	namespace string
	{
		template <typename... Args>
		std::string format(std::string subject, Args... arguments);

		template <typename... Args>
		void str_format(std::string& subject, Args... arguments);

		std::string replace(const std::string search, const std::string replace, std::string subject, const bool first = false);
		void str_replace(const std::string search, const std::string replace, std::string subject, const bool first = false);

		namespace back
		{
			template <typename Type>
			void format(const int iteration, std::string& subject, Type argument);

			template <typename Type, typename... Args>
			void format(const int iteration, std::string& subject, Type argument, Args... arguments);

			template <typename Type>
			void format_replace(const int iteration, const Type argument, std::string& subject);

			void replace(const std::string search, const std::string replace, std::string& subject, const bool first = false);
		}
	}
}

namespace art
{
	namespace string
	{
		template <typename... Args>
		std::string format(std::string subject, Args... arguments)
		{
			back::format(0, subject, arguments...);
			return subject;
		}

		template <typename... Args>
		void str_format(std::string& subject, Args... arguments)
		{
			back::format(0, subject, arguments...);
		}

		inline std::string replace(const std::string search, const std::string replace, std::string subject, const bool first)
		{
			back::replace(search, replace, subject, first);
			return subject;
		}

		inline void str_replace(const std::string search, const std::string replace, std::string subject, const bool first)
		{
			back::replace(search, replace, subject, first);
		}

		namespace back
		{
			template <typename Type>
			void format(const int iteration, std::string& subject, Type argument)
			{
				back::format_replace(iteration, argument, subject);
			}

			template <typename Type, typename... Args>
			void format(const int iteration, std::string& subject, Type argument, Args... arguments)
			{
				back::format_replace(iteration, argument, subject);
				back::format(iteration + 1, subject, arguments...);
			}

			template <typename Type>
			void format_replace(const int iteration, const Type argument, std::string& subject)
			{
				back::replace("{}", argument, subject, true);
				back::replace("{" + std::to_string(iteration) + "}", argument, subject);
			}

			inline void replace(const std::string search, const std::string replace, std::string& subject, const bool first)
			{
				const std::size_t length = search.length();
				std::size_t position = subject.find(search);
				while (position != std::string::npos)
				{
					subject.replace(position, length, replace);
					if (first)
					{
						break;
					}
					position = subject.find(search);
				}
			}
		}
	}
}