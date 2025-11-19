#ifndef APIONEPIECE_H
#define APIONEPIECE_H

#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <pplx/pplx.h>
#include <iostream>
#include <string>
#include <vector>

using namespace web;
using namespace web::http;
using namespace web::http::client;

class Apionepiece {
private:
    std::string api_base = "https://api.api-onepiece.com/v2";
    http_client_config client_config;
    
    http_request create_request(const std::string& endpoint, const std::string& method, const std::string& data = "") {
        http_request request;
        
        if (method == "GET") {
            request.set_method(methods::GET);
        } else if (method == "POST") {
            request.set_method(methods::POST);
        } else if (method == "PUT") {
            request.set_method(methods::PUT);
        } else if (method == "DEL") {
            request.set_method(methods::DEL);
        }
        
        request.set_request_uri(utility::conversions::to_string_t(endpoint));
        
        // Set headers
        request.headers().add(U("Host"), U("api.api-onepiece.com"));
        request.headers().add(U("User-Agent"), U("Mozilla/5.0 (X11; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/115.0"));
        
        if (!data.empty() && (method == "POST" || method == "PUT")) {
            request.set_body(data);
        }
        
        return request;
    }
    
    std::string build_query_params(const std::map<std::string, std::string>& params) {
        if (params.empty()) return "";
        
        std::string query = "?";
        bool first = true;
        for (const auto& param : params) {
            if (!param.second.empty()) {
                if (!first) query += "&";
                // Кодируем значения параметров
                auto encoded_value = web::uri::encode_data_string(utility::conversions::to_string_t(param.second));
                query += param.first + "=" + utility::conversions::to_utf8string(encoded_value);
                first = false;
            }
        }
        return query;
    }
    
    pplx::task<json::value> make_api_call(const std::string& endpoint, const std::string& method, const std::string& data = "") {
        http_client client(utility::conversions::to_string_t(api_base), client_config);
        auto request = create_request(endpoint, method, data);

        return client.request(request)
            .then([](http_response response) {
                if (response.status_code() == status_codes::OK) {
                    return response.extract_json();
                } else {
                    json::value error_obj;
                    error_obj[U("error")] = json::value::string(
                        U("HTTP Error: ") + utility::conversions::to_string_t(std::to_string(response.status_code())));
                    error_obj[U("success")] = json::value::boolean(false);
                    return pplx::task_from_result(error_obj);
                }
            })
            .then([](pplx::task<json::value> previousTask) {
                try {
                    return previousTask.get();
                } catch (const std::exception& e) {
                    json::value error_obj;
                    error_obj[U("error")] = json::value::string(
                        U("Exception: ") + utility::conversions::to_string_t(e.what()));
                    error_obj[U("success")] = json::value::boolean(false);
                    return error_obj;
                }
            });
    }

public:
    Apionepiece(){
        client_config.set_validate_certificates(false);
    }

    pplx::task<json::value> fruits_list() {
        return make_api_call("/fruits/en","GET");
    }

    pplx::task<json::value> fruits_count() {
        return make_api_call("/fruits/en/count","GET");
    }

    pplx::task<json::value> fruit_by_id(int fruits_id) {
        return make_api_call("/fruits/en/" + std::to_string(fruits_id),"GET");
    }

    pplx::task<json::value> search_fruits(const std::string& type="",const std::string& name="") {
        std::map<std::string, std::string> params;
        if (!type.empty()) params["type"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(type)));
        if (!name.empty()) params["name"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(name)));
        return make_api_call("/fruits/en/search/" + build_query_params(params) ,"GET");
    }

    pplx::task<json::value> search_fruits_count(const std::string& type="",const std::string& name="") {
        std::map<std::string, std::string> params;
        if (!type.empty()) params["type"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(type)));
        if (!name.empty()) params["name"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(name)));
        return make_api_call("/fruits/en/search/count" + build_query_params(params) ,"GET");
    }

    pplx::task<json::value> chapters_list() {
        return make_api_call("/chapters/en","GET");
    }

    pplx::task<json::value> chapters_count() {
        return make_api_call("/chapters/en/count","GET");
    }

    pplx::task<json::value> chapter_by_id(int id) {
        return make_api_call("/chapters/en/" + std::to_string(id),"GET");
    }

    pplx::task<json::value> chapter_by_tomeid(int tome_id) {
        return make_api_call("/chapters/en/tome/" + std::to_string(tome_id),"GET");
    }

    pplx::task<json::value> chapter_count_by_tomeid(int tome_id) {
        return make_api_call("/chapters/en/tome/" + std::to_string(tome_id) + "/count","GET");
    }

    pplx::task<json::value> search_chapters(const std::string& title) {
        std::map<std::string, std::string> params;
        if (!title.empty()) params["title"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(title)));
        return make_api_call("/chapters/en/search/" + build_query_params(params) ,"GET");
    }

    pplx::task<json::value> tomes_list() {
        return make_api_call("/tomes/en","GET");
    }

    pplx::task<json::value> tomes_count() {
        return make_api_call("/tomes/en/count","GET");
    }

    pplx::task<json::value> tomes_by_tomeid(int tome_id) {
        return make_api_call("/tomes/en/" + std::to_string(tome_id),"GET");
    }

    pplx::task<json::value> search_tomes(const std::string& title) {
        std::map<std::string, std::string> params;
        if (!title.empty()) params["title"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(title)));
        return make_api_call("/tomes/en/search/" + build_query_params(params) ,"GET");
    }

    pplx::task<json::value> episodes_list() {
        return make_api_call("/episodes/en","GET");
    }

    pplx::task<json::value> episodes_count() {
        return make_api_call("/episodes/en/count","GET");
    }

    pplx::task<json::value> tomes_by_episodeid(int episode_id) {
        return make_api_call("/episodes/en/" + std::to_string(episode_id),"GET");
    }

    pplx::task<json::value> search_episodes(const std::string& title) {
        std::map<std::string, std::string> params;
        if (!title.empty()) params["title"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(title)));
        return make_api_call("/episodes/en/search/" + build_query_params(params) ,"GET");
    }

    pplx::task<json::value> episodes_by_sagaid(int saga_id) {
        return make_api_call("/episodes/en/saga/" + std::to_string(saga_id),"GET");
    }

    pplx::task<json::value> search__by_saga(const std::string& title) {
        std::map<std::string, std::string> params;
        if (!title.empty()) params["title"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(title)));
        return make_api_call("/episodes/en/search/saga/" + build_query_params(params) ,"GET");
    }

    pplx::task<json::value> episodes_count_by_saga(int saga_id) {
        return make_api_call("/episodes/en/count/saga/" + std::to_string(saga_id),"GET");
    }

    pplx::task<json::value> episodes_by_arcid(int arc_id) {
        return make_api_call("/episodes/en/arc/" + std::to_string(arc_id),"GET");
    }

    pplx::task<json::value> search__by_arc(const std::string& title) {
        std::map<std::string, std::string> params;
        if (!title.empty()) params["title"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(title)));
        return make_api_call("/episodes/en/search/arc/" + build_query_params(params) ,"GET");
    }

    pplx::task<json::value> episodes_count_by_arc(int arc_id) {
        return make_api_call("/episodes/en/count/arc/" + std::to_string(arc_id),"GET");
    }

    pplx::task<json::value> dials_list() {
        return make_api_call("/dials/en","GET");
    }

    pplx::task<json::value> dials_count() {
        return make_api_call("/dials/en/count","GET");
    }

    pplx::task<json::value> dials_by_dialsid(int dials_id) {
        return make_api_call("/dials/en/" + std::to_string(dials_id),"GET");
    }

    pplx::task<json::value> search_dials(const std::string& type="",const std::string& name="") {
        std::map<std::string, std::string> params;
        if (!type.empty()) params["type"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(type)));
        if (!name.empty()) params["name"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(name)));
        return make_api_call("/dials/en/search/" + build_query_params(params) ,"GET");
    }

    pplx::task<json::value> movies_list() {
        return make_api_call("/movies/en","GET");
    }

    pplx::task<json::value> movies_count() {
        return make_api_call("/movies/en/count","GET");
    }

    pplx::task<json::value> movie_by_id(int movie_id) {
        return make_api_call("/movies/en/" + std::to_string(movie_id),"GET");
    }

    pplx::task<json::value> search_movies(const std::string& title) {
        std::map<std::string, std::string> params;
        if (!title.empty()) params["title"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(title)));
        return make_api_call("/movies/en/search/" + build_query_params(params) ,"GET");
    }

    pplx::task<json::value> search_movies_count(const std::string& title) {
        std::map<std::string, std::string> params;
        if (!title.empty()) params["title"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(title)));
        return make_api_call("/movies/en/search/count" + build_query_params(params) ,"GET");
    }

    pplx::task<json::value> swords_list() {
        return make_api_call("/swords/en","GET");
    }

    pplx::task<json::value> sword_by_id(int swords_id) {
        return make_api_call("/swords/en/" + std::to_string(swords_id),"GET");
    }

    pplx::task<json::value> search_swords(const std::string& type="",const std::string& name="") {
        std::map<std::string, std::string> params;
        if (!type.empty()) params["type"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(type)));
        if (!name.empty()) params["name"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(name)));
        return make_api_call("/swords/en/search/" + build_query_params(params) ,"GET");
    }

    pplx::task<json::value> swords_state_list() {
        return make_api_call("/swords/en/state","GET");
    }

    pplx::task<json::value> swords_state_count() {
        return make_api_call("/swords/en/state/count","GET");
    }

    pplx::task<json::value> sagas_list() {
        return make_api_call("/sagas/en","GET");
    }

    pplx::task<json::value> sagas_count() {
        return make_api_call("/sagas/en/count","GET");
    }

    pplx::task<json::value> saga_by_id(int saga_id) {
        return make_api_call("/sagas/en/" + std::to_string(saga_id),"GET");
    }

    pplx::task<json::value> search_sagas(const std::string& title) {
        std::map<std::string, std::string> params;
        if (!title.empty()) params["title"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(title)));
        return make_api_call("/sagas/en/search/" + build_query_params(params) ,"GET");
    }

    pplx::task<json::value> locates_list() {
        return make_api_call("/locates/en","GET");
    }

    pplx::task<json::value> locates_count() {
        return make_api_call("/locates/en/count","GET");
    }

    pplx::task<json::value> locates_by_id(int locate_id) {
        return make_api_call("/locates/en/" + std::to_string(locate_id),"GET");
    }

    pplx::task<json::value> search_locates(const std::string& name="",const std::string& sea="",const std::string& affilation="") {
        std::map<std::string, std::string> params;
        if (!name.empty()) params["name"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(name)));
        if (!sea.empty()) params["sea"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(sea)));
        if (!affilation.empty()) params["affilation"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(affilation)));
        return make_api_call("/locates/en/search/" + build_query_params(params) ,"GET");
    }

    pplx::task<json::value> arcs_list() {
        return make_api_call("/arcs/en","GET");
    }

    pplx::task<json::value> arcs_count() {
        return make_api_call("/arcs/en/count","GET");
    }

    pplx::task<json::value> arcs_by_id(int arc_id) {
        return make_api_call("/arcs/en/" + std::to_string(arc_id),"GET");
    }

    pplx::task<json::value> search_arcs(const std::string& title) {
        std::map<std::string, std::string> params;
        if (!title.empty()) params["title"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(title)));
        return make_api_call("/arcs/en/search/" + build_query_params(params) ,"GET");
    }

    pplx::task<json::value> arcs_list_by_sagaid(int saga_id) {
        return make_api_call("/arcs/en/saga/"+ std::to_string(saga_id),"GET");
    }

    pplx::task<json::value> arcs_count_by_sagaid(int saga_id) {
        return make_api_call("/arcs/en/count/saga/"+ std::to_string(saga_id),"GET");
    }

    pplx::task<json::value> boats_list() {
        return make_api_call("/boats/en","GET");
    }

    pplx::task<json::value> boats_count() {
        return make_api_call("/boats/en/count","GET");
    }

    pplx::task<json::value> boats_by_id(int boat_id) {
        return make_api_call("/boats/en/" + std::to_string(boat_id),"GET");
    }

    pplx::task<json::value> search_boats(const std::string& name="",const std::string& type="") {
        std::map<std::string, std::string> params;
        if (!name.empty()) params["name"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(name)));
        if (!type.empty()) params["type"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(type)));
        return make_api_call("/boats/en/search/" + build_query_params(params) ,"GET");
    }

    pplx::task<json::value> boats_by_crewid(int crew_id) {
        return make_api_call("/boats/en/crew/" + std::to_string(crew_id),"GET");
    }

    pplx::task<json::value> boats_count_by_crewid(int crew_id) {
        return make_api_call("/boats/en/count/crew/" + std::to_string(crew_id),"GET");
    }

    pplx::task<json::value> boats_by_captainid(int captain_id) {
        return make_api_call("/boats/en/captain/" + std::to_string(captain_id),"GET");
    }

    pplx::task<json::value> boats_count_by_captainid(int captain_id) {
        return make_api_call("/boats/en/count/captain/" + std::to_string(captain_id),"GET");
    }

    pplx::task<json::value> characters_list() {
        return make_api_call("/characters/en","GET");
    }

    pplx::task<json::value> characters_count() {
        return make_api_call("/characters/en/count","GET");
    }

    pplx::task<json::value> characters_by_id(int boat_id) {
        return make_api_call("/boats/en/" + std::to_string(boat_id),"GET");
    }

    pplx::task<json::value> search_characters(const std::string& name="",const std::string& job="",const std::string& bounty="",const std::string& age="",const std::string& size="") {
        std::map<std::string, std::string> params;
        if (!name.empty()) params["name"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(name)));
        if (!job.empty()) params["job"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(job)));
        if (!bounty.empty()) params["bounty"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(bounty)));
        if (!age.empty()) params["age"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(age)));
        if (!size.empty()) params["size"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(size)));
        return make_api_call("/characters/en/search/" + build_query_params(params) ,"GET");
    }

    pplx::task<json::value> characters_by_crewid(int crew_id) {
        return make_api_call("/characters/en/crew/" + std::to_string(crew_id),"GET");
    }

    pplx::task<json::value> characters_count_by_crewid(int crew_id) {
        return make_api_call("/characters/en/count/crew/" + std::to_string(crew_id) + "/count","GET");
    }

    pplx::task<json::value> crews_list() {
        return make_api_call("/crews/en","GET");
    }

    pplx::task<json::value> crews_count() {
        return make_api_call("/crews/en/count","GET");
    }

    pplx::task<json::value> crews_by_id(int crew_id) {
        return make_api_call("/crews/en/" + std::to_string(crew_id),"GET");
    }

    pplx::task<json::value> search_crews(const std::string& name="",const std::string& status="") {
        std::map<std::string, std::string> params;
        if (!name.empty()) params["name"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(name)));
        if (!status.empty()) params["status"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(status)));
        return make_api_call("/crews/en/search/" + build_query_params(params) ,"GET");
    }

    pplx::task<json::value> crews_yonko_list() {
        return make_api_call("/crews/yonko","GET");
    }

    pplx::task<json::value> crews_yonko_count() {
        return make_api_call("/crews/yonko/count","GET");
    }

    pplx::task<json::value> luffy_technique_list() {
        return make_api_call("/luffy-techniques/en","GET");
    }

    pplx::task<json::value> luffy_technique_count() {
        return make_api_call("/luffy-techniques/en/count","GET");
    }

    pplx::task<json::value> luffy_technique_id(int tech_id) {
        return make_api_call("/luffy-techniques/en/" + std::to_string(tech_id),"GET");
    }

    pplx::task<json::value> search_luffy_technique(const std::string& name="",const std::string& translation="") {
        std::map<std::string, std::string> params;
        if (!name.empty()) params["name"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(name)));
        if (!translation.empty()) params["translation"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(translation)));
        return make_api_call("/luffy-techniques/en/search/" + build_query_params(params) ,"GET");
    }

    pplx::task<json::value> luffy_technique_by_gearid(int gear_id) {
        return make_api_call("/luffy-techniques/en/gear/" + std::to_string(gear_id),"GET");
    }

    pplx::task<json::value> characters_count_by_gearid(int gear_id) {
        return make_api_call("/luffy-techniques/en/gear/" + std::to_string(gear_id) + "/count","GET");
   }


    pplx::task<json::value> luffy_gears_list() {
        return make_api_call("/luffy-gears/en","GET");
    }

    pplx::task<json::value> luffy_gears_count() {
        return make_api_call("/luffy-gears/en/count","GET");
    }

    pplx::task<json::value> luffy_gears_by_id(int gears_id) {
        return make_api_call("/luffy-gears/en/" + std::to_string(gears_id),"GET");
    }

    pplx::task<json::value> hakis_list() {
        return make_api_call("/hakis/en","GET");
    }

    pplx::task<json::value> hakis_count() {
        return make_api_call("/hakis/en/count","GET");
    }

    pplx::task<json::value> hakis_by_id(int hakis_id) {
        return make_api_call("/hakis/en/" + std::to_string(hakis_id),"GET");
    }

    pplx::task<json::value> search_hakis(const std::string& name="",const std::string& roman_name="") {
        std::map<std::string, std::string> params;
        if (!name.empty()) params["name"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(name)));
        if (!roman_name.empty()) params["roman_name"] = utility::conversions::to_utf8string(web::uri::encode_data_string(utility::conversions::to_string_t(roman_name)));
        return make_api_call("/hakis/en/search/" + build_query_params(params) ,"GET");
    }

    pplx::task<json::value> hakis_by_characterid(int character_id) {
        return make_api_call("/hakis/en/character/" + std::to_string(character_id),"GET");
    }

    pplx::task<json::value> hakis_count_by_characterid(int character_id) {
        return make_api_call("/hakis/en/character/" + std::to_string(character_id) + "/count","GET");
    }
};

#endif
