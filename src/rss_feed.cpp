#include "../include/rss_feed.hpp"
#include <iostream>
#include <sstream>
#include <regex>
#include <fstream>
#include <cstdlib>
#include <curl/curl.h>

// Callback function for libcurl to write data
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

RSSFeed::RSSFeed() : curlHandle(nullptr) {
    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curlHandle = curl_easy_init();

    if (!curlHandle) {
        lastError = "Failed to initialize libcurl";
    }
}

RSSFeed::~RSSFeed() {
    // Clean up libcurl
    if (curlHandle) {
        curl_easy_cleanup(static_cast<CURL*>(curlHandle));
    }
    curl_global_cleanup();
}

std::vector<NewsItem> RSSFeed::fetchNews(int count) {
    // New York Times RSS feed URL for World news
    const std::string url = "https://rss.nytimes.com/services/xml/rss/nyt/World.xml";

    try {
        // Fetch the RSS feed
        std::string data = fetchURL(url);

        // If fetching failed, use sample data as fallback
        if (data.empty()) {
            std::cout << "Warning: Using sample data as fallback." << std::endl;
            data = R"(
            <rss version="2.0">
            <channel>
                <title>AP Top News</title>
                <link>https://apnews.com</link>
                <description>The latest headlines from the Associated Press</description>
                <item>
                    <title>Sample News Item 1</title>
                    <link>https://apnews.com/article/sample1</link>
                    <description>This is a sample news item for demonstration purposes.</description>
                    <pubDate>Mon, 10 Apr 2023 12:00:00 GMT</pubDate>
                    <guid>https://apnews.com/article/sample1</guid>
                </item>
                <item>
                    <title>Sample News Item 2</title>
                    <link>https://apnews.com/article/sample2</link>
                    <description>Another sample news item with some content.</description>
                    <pubDate>Mon, 10 Apr 2023 11:30:00 GMT</pubDate>
                    <guid>https://apnews.com/article/sample2</guid>
                </item>
                <item>
                    <title>Sample News Item 3</title>
                    <link>https://apnews.com/article/sample3</link>
                    <description>A third sample news item with different content.</description>
                    <pubDate>Mon, 10 Apr 2023 11:00:00 GMT</pubDate>
                    <guid>https://apnews.com/article/sample3</guid>
                </item>
                <item>
                    <title>Sample News Item 4</title>
                    <link>https://apnews.com/article/sample4</link>
                    <description>A fourth sample news item for testing.</description>
                    <pubDate>Mon, 10 Apr 2023 10:30:00 GMT</pubDate>
                    <guid>https://apnews.com/article/sample4</guid>
                </item>
                <item>
                    <title>Sample News Item 5</title>
                    <link>https://apnews.com/article/sample5</link>
                    <description>A fifth sample news item to complete the set.</description>
                    <pubDate>Mon, 10 Apr 2023 10:00:00 GMT</pubDate>
                    <guid>https://apnews.com/article/sample5</guid>
                </item>
            </channel>
            </rss>
            )";
        }

        // Parse the RSS feed
        return parseRSS(data, count);
    } catch (const std::exception& e) {
        lastError = e.what();
        return {};
    }
}

void RSSFeed::setProgressCallback(std::function<void(int, int)> callback) {
    progressCallback = callback;
}

std::string RSSFeed::getLastError() const {
    return lastError;
}

std::string RSSFeed::fetchURL(const std::string& url) {
    std::cout << "Fetching URL: " << url << std::endl;

    if (!curlHandle) {
        lastError = "libcurl not initialized";
        std::cerr << "Error: " << lastError << std::endl;
        return "";
    }

    CURL* curl = static_cast<CURL*>(curlHandle);
    std::string readBuffer;

    // Set up the request
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);

    // Disable SSL verification for testing purposes
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    // Perform the request
    CURLcode res = curl_easy_perform(curl);

    // Check for errors
    if (res != CURLE_OK) {
        lastError = std::string("curl_easy_perform() failed: ") +
                   curl_easy_strerror(res);
        std::cerr << "Error fetching URL: " << lastError << std::endl;
        return "";
    }

    return readBuffer;
}

std::vector<NewsItem> RSSFeed::parseRSS(const std::string& data, int count) {
    std::vector<NewsItem> items;

    // Simple regex-based parsing for demonstration purposes
    // In a real application, you would use a proper XML parser like libxml2

    // Find all item elements
    // Use a simple string search approach instead of regex for multiline matching
    std::string itemStart = "<item>";
    std::string itemEnd = "</item>";
    std::regex titleRegex("<title>(.*?)</title>");
    std::regex linkRegex("<link>(.*?)</link>");
    std::regex descRegex("<description>(.*?)</description>");
    std::regex dateRegex("<pubDate>(.*?)</pubDate>");
    std::regex guidRegex("<guid>(.*?)</guid>");

    int itemCount = 0;
    size_t pos = 0;

    while (itemCount < count) {
        // Find the start of the next item
        size_t itemStartPos = data.find(itemStart, pos);
        if (itemStartPos == std::string::npos) {
            break; // No more items
        }

        // Find the end of the item
        size_t itemEndPos = data.find(itemEnd, itemStartPos);
        if (itemEndPos == std::string::npos) {
            break; // Malformed XML
        }

        // Extract the item content (including the tags)
        std::string itemContent = data.substr(itemStartPos, itemEndPos - itemStartPos + itemEnd.length());

        // Update position for next search
        pos = itemEndPos + itemEnd.length();

        NewsItem item;

        // Extract title
        std::smatch titleMatch;
        if (std::regex_search(itemContent, titleMatch, titleRegex) && titleMatch.size() > 1) {
            item.title = titleMatch.str(1);
        }

        // Extract link
        std::smatch linkMatch;
        if (std::regex_search(itemContent, linkMatch, linkRegex) && linkMatch.size() > 1) {
            item.link = linkMatch.str(1);
        }

        // Extract description
        std::smatch descMatch;
        if (std::regex_search(itemContent, descMatch, descRegex) && descMatch.size() > 1) {
            item.description = descMatch.str(1);

            // Clean up HTML tags from description
            item.description = std::regex_replace(item.description, std::regex("<[^>]*>"), "");
        }

        // Extract publication date
        std::smatch dateMatch;
        if (std::regex_search(itemContent, dateMatch, dateRegex) && dateMatch.size() > 1) {
            item.pubDate = dateMatch.str(1);
        }

        // Extract GUID
        std::smatch guidMatch;
        if (std::regex_search(itemContent, guidMatch, guidRegex) && guidMatch.size() > 1) {
            item.guid = guidMatch.str(1);
        }

        items.push_back(item);
        itemCount++;

        // Call progress callback if set
        if (progressCallback) {
            progressCallback(itemCount, count);
        }
    }

    return items;
}
