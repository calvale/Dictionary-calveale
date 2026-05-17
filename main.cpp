// TODO File block comment

// TODO If you'd like to include any other header files, do so right below
// this comment


/* BEGINNING OF TEMPLATE CODE; DO NOT MODIFY */




#include <sstream>
#include <iostream>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "cpp-httplib/httplib.h"

#include "json11/json11.hpp"

const std::string DICTIONARY_API_HOST = "https://api.dictionaryapi.dev";
const std::string DICTIONARY_API_PATH_PREFIX = "/api/v2/entries/en/";

using json11::Json;

/*
 * Function: build_request_endpoint
 * Description: Builds the dictionaryapi.dev request endpoint for a given word
 * Parameters:
 * 		word (std::string): Word for which to build request endpoint
 * Returns (std::string): Request endpoint
 */
std::string build_request_endpoint(std::string word) {
	std::stringstream endpoint_ss;
	endpoint_ss << DICTIONARY_API_PATH_PREFIX;
	endpoint_ss << word;
	return endpoint_ss.str();
}

/*
 * Function: extract_definition
 * Description: Extracts the first definition available in the given
 * 		JSON response from dictionaryapi.dev.
 * Parameters:
 * 		words (json11:Json): Parsed json11::Json object of response from
 * 			dictionaryapi.dev
 * Returns (std::string): First definition available in given JSON response.
 * 		If none are available, returns an empty string.
 */
std::string extract_definition(const Json words) {
	// For each word
	const Json::array& word_arr = words.array_items();
	for (const Json& word : word_arr) {
		// Get meanings
		const Json& meanings = word["meanings"];

		// For each meaning
		const Json::array& meaning_arr = meanings.array_items();
		for (const Json& meaning : meaning_arr) {
			// Get thesaurus entries
			const Json thesaurus_entries = meaning["definitions"];

			// For each thesaurus entry
			const Json::array& thesaurus_entry_arr =
				thesaurus_entries.array_items();
			for (const Json& thesaurus_entry : thesaurus_entry_arr) {
				// Get definition
				const std::string definition =
					thesaurus_entry["definition"].string_value();
				
				if (!definition.empty()) {
					// If definition is not empty, return it.
					return definition;
				}

				// Otherwise, keep looking for a non-empty definition.
			}

		}
	}

	// Failed to find definition in response. Return empty string.
	return "";
}

/*
 * Function: query_definition
 * Description: Queries the definition of a given word and returns it
 * 		if the word exists in the dictionaryapi.dev database. Else, it returns
 * 		an empty string.
 * Parameters:
 * 		word (std::string): Word for which to query definition
 * Returns (string): Definition of word if found, or empty string otherwise.
 */
std::string query_definition(std::string word) {
	// Construct http client
	httplib::Client cli(DICTIONARY_API_HOST);

	// Make request
	auto res = cli.Get(build_request_endpoint(word));

	if (res->status != 200) {
		// If status is not 200 (OK), then the word does not exist in the
		// dictionary API's database. Return empty string for definition
		return "";
	}

	// Parse body to Json (array of words)
	std::string err;
	const Json words = Json::parse(res->body, err);
	if (!err.empty()) {
		// If an error occurred in parsing, return an empty string for
		// definition.
		return "";
	}

	// Extract definition
	std::string definition = extract_definition(words);

	return definition;
}

/*
 * Function: extract_audio_url
 * Description: Extracts the first dictionaryapi.dev phonetic audio sample URL
 * 		available from the JSON response.
 * Parameters:
 * 		words (json11:Json): Parsed json11::Json object of response from
 * 			dictionaryapi.dev
 * Returns (std::string): Audio sample URL. If none are available, returns
 * 		an empty string.
 */
std::string extract_audio_url(const Json words) {
	// For each word
	const Json::array& word_arr = words.array_items();
	for (const Json& word : word_arr) {
		// Get phonetics
		const Json phonetics = word["phonetics"];

		// For each phonetic entry
		const Json::array& phonetic_arr = phonetics.array_items();
		for (const Json& phonetic : phonetic_arr) {
			// Get audio url
			const std::string audio_url = phonetic["audio"].string_value();

			if (!audio_url.empty()) {
				// If audio_url is not empty, return it.
				return audio_url;
			}

			// Otherwise, keep looking for a non-empty URL.
		}
	}

	// Failed to find audio URL in response. Return empty string.
	return "";
}

/*
 * Function: query_audio_url
 * Description: Queries the dictionaryapi.dev phonetic audio sample URL of a
 * 		given word and returns it if the word exists in the dictionaryapi.dev
 * 		database. Else, it returns an empty string.
 * Parameters:
 * 		word (std::string): Word for which to query audio sample URL
 * Returns (string): Audio sample URL of word if found. If the word is not found
 * 		in the database or the word doesn't have an audio sample URL associated
 * 		with it, this function returns an empty string.
 */
std::string query_audio_url(std::string word) {
	// Construct http client
	httplib::Client cli(DICTIONARY_API_HOST);

	// Make request
	auto res = cli.Get(build_request_endpoint(word));

	if (res->status != 200) {
		// If status is not 200 (OK), then the word does not exist in the
		// dictionary API's database. Return empty string for definition
		return "";
	}

	// Parse body to Json (array of words)
	std::string err;
	const Json words = Json::parse(res->body, err);
	if (!err.empty()) {
		// If an error occurred in parsing, return an empty string for
		// definition.
		return "";
	}

	// Extract audio URL
	std::string audio_url = extract_audio_url(words);

	return audio_url;
}

/*
 * Function: extract_pronunciation
 * Description: Extracts the first available formal pronunciation from the
 * 		dictionaryapi.dev JSON response.
 * Parameters:
 * 		words (json11:Json): Parsed json11::Json object of response from
 * 			dictionaryapi.dev
 * Returns (std::string): First available pronunciation in JSON response. If
 * 		none are available, returns an empty string.
 */
std::string extract_pronunciation(const Json words) {
	// For each word
	const Json::array& word_arr = words.array_items();
	for (const Json& word : word_arr) {
		// Get pronunciation
		const std::string pronunciation = word["phonetic"].string_value();

		if (!pronunciation.empty()) {
			// If pronunciation is not empty, return it
			return pronunciation;
		}

		// Otherwise, check the ["phonetics"]["text"] fields

		// Get phonetics
		const Json phonetics = word["phonetics"];

		// For each phonetic entry
		const Json::array& phonetic_arr = phonetics.array_items();
		for (const Json& phonetic : phonetic_arr) {
			// Get text of phonetic
			const std::string pronunciation = phonetic["text"].string_value();

			if (!pronunciation.empty()) {
				// If pronunciation is not empty, return it.
				return pronunciation;
			}

			// Otherwise, keep looking for a non-empty pronunciation
		}
	}

	// Failed to find pronunciation in response. Return empty string.
	return "";
}

/*
 * Function: query_pronunciation
 * Description: Queries the dictionaryapi.dev formal pronunciation text of a
 * 		given word and returns it if the word exists in the dictionaryapi.dev
 * 		database. Else, it returns an empty string.
 * Parameters:
 * 		word (std::string): Word for which to query pronunciation
 * Returns (string): Pronunciation of word if found. Else, returns an empty
 * 		string.
 */
std::string query_pronunciation(std::string word) {
	// Construct http client
	httplib::Client cli(DICTIONARY_API_HOST);

	// Make request
	auto res = cli.Get(build_request_endpoint(word));

	if (res->status != 200) {
		// If status is not 200 (OK), then the word does not exist in the
		// dictionary API's database. Return empty string for definition
		return "";
	}

	// Parse body to Json (array of words)
	std::string err;
	const Json words = Json::parse(res->body, err);
	if (!err.empty()) {
		// If an error occurred in parsing, return an empty string for
		// definition.
		return "";
	}

	// Extract pronunciation
	std::string pronunciation = extract_pronunciation(words);

	return pronunciation;
}


/* END OF TEMPLATE CODE */

/*
 * Name: Aleena Calvert
 * Assignment: Dictionary
 * Description: This program lets the user look up a word's definition,
 * audio URL, or pronunciation using the dictionary API starter functions.
 */

#include <sstream>
#include <iostream>

/*
 * Function: print_menu
 * Description: Prints the dictionary menu options.
 * Parameters: None
 * Returns: Nothing
 */

void print_menu()
{
    std::cout << std::endl;
    std::cout << "Dictionary Menu" << std::endl;
    std::cout << "1. Look up definition" << std::endl;
    std::cout << "2. Look up audio URL" << std::endl;
    std::cout << "3. Look up pronunciation" << std::endl;
    std::cout << "4. Exit" << std::endl;
    std::cout << "Enter your choice: ";
}

/*
 * Function: get_word
 * Description: Prompts the user for a word.
 * Parameters: None
 * Returns: The word entered by the user.
 */
std::string get_word()
{
    std::string word;

    std::cout << "Enter a word: ";
    std::cin >> word;

    return word;
}

/*
 * Function: print_answer
 * Description: Prints the result or an error message if nothing was found.
 * Parameters:
 *      answer: The information returned from the query function.
 *      label: The type of information being printed.
 * Returns: Nothing
 */
void print_answer(std::string answer, std::string label)
{
    if (answer == "") {
        std::cout << "I could not find that information." << std::endl;
    } else {
        std::cout << label << ": " << answer << std::endl;
    }
}

int main()
{
    std::string choice;
    std::string word;
    std::string answer;

    while (choice != "4") {

        print_menu();
        std::cin >> choice;

        if (choice == "1") {

            word = get_word();

            answer = query_definition(word);

            print_answer(answer, "Definition");

        } else if (choice == "2") {

            word = get_word();

            answer = query_audio_url(word);

            print_answer(answer, "Audio URL");

        } else if (choice == "3") {

            word = get_word();

            answer = query_pronunciation(word);

            print_answer(answer, "Pronunciation");

        } else if (choice == "4") {

            std::cout << "Goodbye!" << std::endl;

        } else {

            std::cout << "That is not a valid choice. Try again." << std::endl;

        }
    }

    return 0;
}
