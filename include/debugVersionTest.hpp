#pragma once

#include <SFML/Network.hpp>
#include <dialogue.hpp>

void test_debug_version(const float& current_version)
{
    sf::Http http;
    http.setHost("http://version-validation.glitch.me/");
    sf::Http::Request request;
    sf::Http::Response response = http.sendRequest(request);
    if(response.getStatus() != sf::Http::Response::Ok && response.getStatus() != sf::Http::Response::Accepted)
    {
        std::cout << "Validation failed\n";
        exit(1);
    }
    std::string body = response.getBody();
    std::string version_str = "<h1 class=\"title\">version=";
    size_t version_idx = body.find(version_str) + version_str.size();
    size_t version_end_idx = body.find("</h1>");
    version_str = body.substr(version_idx, version_end_idx - version_idx);
    float version = atof(version_str.c_str());
    if(version <= current_version) return;
    std::string v_num = std::to_string(version), current_v_num = std::to_string(current_version);
    v_num.erase(v_num.find_last_not_of('0') + 1, std::string::npos);
    current_v_num.erase(current_v_num.find_last_not_of('0') + 1, std::string::npos);
#ifndef SERVER
#ifdef WINDOWS
    std::string path = "tools\\dialogue\\dialogue.exe";
#else
    std::string path = "tools/dialogue/dialogue";
#endif
    dialogue(path, "Development Build", (std::string("This development build is outdated.\nThe minimum supported version is v") + v_num + std::string("\nCurrent version: v") + current_v_num).c_str(), get_option(Ok));
#endif
    std::cout << "- Development Build -\nThis development build is outdated.\nThe minimum supported version is v" << v_num << "\nCurrent version: v" << current_v_num << "\n";
    exit(1);
}
