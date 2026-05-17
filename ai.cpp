#include <iostream>
#include <string>

// Forward declarations for the API functions provided in the starter code
std::string query_definition(const std::string& word);
std::string query_audio(const std::string& word);
std::string query_pronunciation(const std::string& word);

// Helper function to keep the main loop clean
void displayMenu() {
    std::cout << "\n========================================\n";
    std::cout << "        Free Dictionary API Tool        \n";
    std::cout << "========================================\n";
    std::cout << "1. Look up the definition of a word\n";
    std::cout << "2. Look up a URL to an audio sample\n";
    std::cout << "3. Look up the formal pronunciation\n";
    std::cout << "4. Exit the program\n";
    std::cout << "Please select an option (1-4): ";
}

int main() {
    int userChoice; 
    std::string targetWord;
    std::string result;

    while (true) {
        displayMenu();
        
        // Read user input
        std::cin >> userChoice;

        // Check if the user wants to exit
        if (userChoice == 4) {
            std::cout << "Exiting the program. Goodbye!\n";
            break;
        }

        // Process valid dictionary lookups
        if (userChoice >= 1 && userChoice <= 3) {
            std::cout << "Enter the word you wish to look up: ";
            std::cin >> targetWord;

            // Route to the correct API function using a switch statement
            switch (userChoice) {
                case 1:
                    result = query_definition(targetWord);
                    break;
                case 2:
                    result = query_audio(targetWord);
                    break;
                case 3:
                    result = query_pronunciation(targetWord);
                    break;
            }

            // Display the results
            if (result.empty()) {
                std::cout << "\n[Error] We could not find the requested information for '" << targetWord << "'.\n";
            } else {
                std::cout << "\n--- Result ---\n" << result << "\n--------------\n";
            }
        } 
        // Handle invalid menu options
        else {
            std::cout << "\n[Invalid Input] Please read the menu carefully and enter a number between 1 and 4.\n";
        }
    }
