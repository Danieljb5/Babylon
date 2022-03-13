#pragma once

#include <iostream>
#include <future>

enum option
{
    Ok, Cancel, Yes, No, option_count
};

namespace detail
{
    const char* option_name[]
    {
        "OK", "Cancel", "Yes", "No"
    };

    std::string path = "dialogue/dialogue";

    std::string parse_str(std::string str)
    {
        std::string result;
        for(int i = 0; i < str.size(); i++)
        {
            if(str[i] == ' ')
            {
                result += "\\\\";
                result += "s";
            }
            else if(str[i] == '\n')
            {
                result += "\\\\";
                result += "n";
            }
            else
            {
                result += str[i];
            }
        }
        return result;
    }

    int get_status(int exit_sig)
    {
    #if defined (__LINUX__) || defined (__gnu_linux__) || defined (__linux__)
        return WEXITSTATUS(exit_sig);
    #elif defined _WIN32
        return exit_sig;
    #else
        return -1;
    #endif
    }
}

const char* get_option(option o)
{
    return (detail::option_name[o]);
}

int dialogue(std::string path, const char* title, const char* message)
{
    std::string str_title = detail::parse_str(title);
    std::string str_message = detail::parse_str(message);
    int status = system(std::string(path + std::string(" ") + str_title + std::string(" ") + str_message).c_str());
    return detail::get_status(status);
}

int dialogue(std::string path, const char* title, const char* message, const char* options)
{
    std::string str_title = detail::parse_str(title);
    std::string str_message = detail::parse_str(message);
    int status = system(std::string(path + std::string(" ") + str_title + std::string(" ") + str_message + std::string(" ") + std::string(options)).c_str());
    return detail::get_status(status);
}

int dialogue(const char* title, const char* message)
{
    std::string str_title = detail::parse_str(title);
    std::string str_message = detail::parse_str(message);
    int status = system(std::string(detail::path + std::string(" ") + str_title + std::string(" ") + str_message).c_str());
    return detail::get_status(status);
}

int dialogue(const char* title, const char* message, const char* options)
{
    std::string str_title = detail::parse_str(title);
    std::string str_message = detail::parse_str(message);
    int status = system(std::string(detail::path + std::string(" ") + str_title + std::string(" ") + str_message + std::string(" ") + std::string(options)).c_str());
    return detail::get_status(status);
}

namespace detail
{
    int dialogue(std::string path, const char* title, const char* message, const char* options)
    {
        return ::dialogue(path, title, message, options);
    }
}

std::future<int> dialogue_async(std::string path, const char* title, const char* message, const char* options)
{
    std::future<int> f = std::async(std::launch::async, detail::dialogue, path, title, message, options);
    return f;
}

std::future<int> dialogue_async(std::string path, const char* title, const char* message)
{
    std::future<int> f = std::async(std::launch::async, detail::dialogue, path, title, message, "");
    return f;
}

std::future<int> dialogue_async(const char* title, const char* message, const char* options)
{
    std::future<int> f = std::async(std::launch::async, detail::dialogue, detail::path, title, message, options);
    return f;
}

std::future<int> dialogue_async(const char* title, const char* message)
{
    std::future<int> f = std::async(std::launch::async, detail::dialogue, detail::path, title, message, "");
    return f;
}

void set_path(std::string path)
{
    detail::path = path;
}