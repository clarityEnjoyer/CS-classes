// Author: Aliaksei Papkouski 484417 MIMUW
#include <algorithm>
#include <array>
#include <charconv>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <optional>
#include <ranges>
#include <regex>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>

namespace {

constexpr std::string_view WHITESPACE = " \t";
constexpr std::size_t MEDAL_CATEGORIES_COUNT = 4;
constexpr std::size_t ANY_MEDAL = 0;
constexpr std::uint16_t ANY_YEAR = 0;
constexpr std::string_view ANY_EVENT = "";

// Domain type aliases
using Medal = std::size_t;      // 0 = (any medal), 1..3 = specific medal
using Year = std::uint16_t;     // 0 = (any year), 1800..59999 = specific year
using EventName = std::string;  // "" = (any event)

// Data structure: Event -> Year -> Array of 4 medal counters [0..3]
using MedalCounts = std::array<std::size_t, MEDAL_CATEGORIES_COUNT>;
using YearMap = std::unordered_map<Year, MedalCounts>;
using Database = std::unordered_map<EventName, YearMap>;

// Represents parsed criteria from a single input line
using Criteria = std::tuple<Medal, Year, EventName>;

void communicateError(std::size_t lineNumber) {
    std::cerr << "ERROR " << lineNumber << '\n';
}

void trimLeadingWhitespace(std::string_view &line) {
    std::size_t position = line.find_first_not_of(WHITESPACE);
    if (position == std::string_view::npos) {
        line = {};
    }
    else {
        line.remove_prefix(position);
    }
}

void standardizeEventName(EventName &eventName) {
    std::ranges::replace(eventName, '\t', ' ');
    auto [newEnd, end] = std::ranges::unique(eventName, [](char a, char b) {
        return a == ' ' && b == ' ';
    });
    eventName.erase(newEnd, end);
}

bool matchesPrefix(
    std::string_view line,
    const std::regex &pattern,
    std::match_results<std::string_view::const_iterator> &matches
) {
    return std::regex_search(line.begin(), line.end(), matches, pattern);
}

// Common token parser for both queries ('?') and data insertion lines.
// Returns std::nullopt if the line contains invalid tokens,
// glued words, or duplicate categories.
std::optional<Criteria> extractCriteria(std::string_view line) {
    // Positive lookahead (?=[ \t]|$) at the end of each regex guarantees
    // that every token is followed by whitespace or the end of the line.
    static const std::regex MEDAL_REGEX(R"(^[1-3](?=[ \t]|$))");
    static const std::regex YEAR_REGEX(
        R"(^(?:1[89]\d{2}|[2-9]\d{3}|[1-5]\d{4})(?=[ \t]|$))"
    );
    static const std::regex EVENT_NAME_REGEX(
        R"(^[a-zA-Z]+(?:[ \t]+[a-zA-Z]+)*(?=[ \t]|$))"
    );

    Medal medal = ANY_MEDAL;
    Year year = ANY_YEAR;
    EventName eventName{ANY_EVENT};
    std::match_results<std::string_view::const_iterator> matches;

    while (true) {
        trimLeadingWhitespace(line);
        if (line.empty()) {
            break;
        }

        if (matchesPrefix(line, MEDAL_REGEX, matches)) {
            if (medal != ANY_MEDAL) {
                return std::nullopt; // Duplicate medal criterion
            }
            medal = static_cast<Medal>(line.front() - '0');
            line.remove_prefix(matches[0].length());
        }
        else if (matchesPrefix(line, YEAR_REGEX, matches)) {
            if (year != ANY_YEAR) {
                return std::nullopt; // Duplicate year criterion
            }
            std::size_t length = static_cast<std::size_t>(matches[0].length());
            std::from_chars(line.data(), line.data() + length, year);
            line.remove_prefix(length);
        }
        else if (matchesPrefix(line, EVENT_NAME_REGEX, matches)) {
            if (!eventName.empty()) {
                // Duplicate or split event name (e.g., "Olympic 1972 Games")
                return std::nullopt;
            }
            eventName = matches[0].str();
            standardizeEventName(eventName);
            line.remove_prefix(matches[0].length());
        }
        else {
            return std::nullopt; // Unrecognized token (garbage in input)
        }
    }

    return Criteria{medal, year, std::move(eventName)};
}

// Updates all 8 combinations in the data structure
// (combining specific values and sentinels).
void insertRecord(Database &database, const Criteria &criteria) {
    const auto &[medal, year, eventName] = criteria;
    const std::array<std::string_view, 2> eventKeys = {ANY_EVENT, eventName};
    const std::array<Year, 2> yearKeys = {ANY_YEAR, year};

    for (std::string_view eventKey : eventKeys) {
        auto &yearMap = database[std::string(eventKey)];
        for (Year yearKey : yearKeys) {
            auto &medalArray = yearMap[yearKey];
            ++medalArray[ANY_MEDAL];
            ++medalArray[medal];
        }
    }
}

// Performs a read-only lookup via const reference to prevent accidental
// insertion of empty keys by operator[].
std::size_t queryDatabase(const Database &database, const Criteria &criteria) {
    const auto &[medal, year, eventName] = criteria;

    auto eventIt = database.find(eventName);
    if (eventIt == database.end()) {
        return 0;
    }

    const YearMap &yearMap = eventIt->second;
    auto yearIt = yearMap.find(year);
    if (yearIt == yearMap.end()) {
        return 0;
    }

    return yearIt->second[medal];
}

void processLine(
    std::string_view line,
    std::size_t lineNumber,
    Database &database
) {
    trimLeadingWhitespace(line);
    if (line.empty()) {
        communicateError(lineNumber);
        return;
    }

    const bool isQuery = (line.front() == '?');
    if (isQuery) {
        line.remove_prefix(1);
        // The '?' token must be followed by either EOL or whitespace
        // (rejects e.g. "?1972" or "??").
        if (!line.empty() && !WHITESPACE.contains(line.front())) {
            communicateError(lineNumber);
            return;
        }
    }

    auto criteriaOpt = extractCriteria(line);
    if (!criteriaOpt.has_value()) {
        communicateError(lineNumber);
        return;
    }

    if (isQuery) {
        std::cout << queryDatabase(database, *criteriaOpt) << '\n';
    }
    else {
        const auto &[medal, year, eventName] = *criteriaOpt;
        // All three criteria are mandatory when inserting a new medal record.
        if (medal == ANY_MEDAL || year == ANY_YEAR || eventName.empty()) {
            communicateError(lineNumber);
            return;
        }

        insertRecord(database, *criteriaOpt);
    }
}

} // namespace

int main() {
    Database database;
    std::string currentLine;
    std::size_t lineNumber = 0;

    while (std::getline(std::cin, currentLine)) {
        processLine(currentLine, ++lineNumber, database);
    }

    return 0;
}