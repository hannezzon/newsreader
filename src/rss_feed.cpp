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
                <title>NYT > World News</title>
                <link>https://www.nytimes.com/section/world</link>
                <description>The latest news from around the world from the New York Times</description>
                <item>
                    <title>Sample News Item 1</title>
                    <link>https://www.nytimes.com/article/sample1</link>
                    <description>This is a sample news item for demonstration purposes.</description>
                    <pubDate>Mon, 10 Apr 2023 12:00:00 GMT</pubDate>
                    <guid>https://www.nytimes.com/article/sample1</guid>
                </item>
                <item>
                    <title>Sample News Item 2</title>
                    <link>https://www.nytimes.com/article/sample2</link>
                    <description>Another sample news item with some content.</description>
                    <pubDate>Mon, 10 Apr 2023 11:30:00 GMT</pubDate>
                    <guid>https://www.nytimes.com/article/sample2</guid>
                </item>
                <item>
                    <title>Sample News Item 3</title>
                    <link>https://www.nytimes.com/article/sample3</link>
                    <description>A third sample news item with different content.</description>
                    <pubDate>Mon, 10 Apr 2023 11:00:00 GMT</pubDate>
                    <guid>https://www.nytimes.com/article/sample3</guid>
                </item>
                <item>
                    <title>Sample News Item 4</title>
                    <link>https://www.nytimes.com/article/sample4</link>
                    <description>A fourth sample news item for testing.</description>
                    <pubDate>Mon, 10 Apr 2023 10:30:00 GMT</pubDate>
                    <guid>https://www.nytimes.com/article/sample4</guid>
                </item>
                <item>
                    <title>Sample News Item 5</title>
                    <link>https://www.nytimes.com/article/sample5</link>
                    <description>A fifth sample news item to complete the set.</description>
                    <pubDate>Mon, 10 Apr 2023 10:00:00 GMT</pubDate>
                    <guid>https://www.nytimes.com/article/sample5</guid>
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
    if (!curlHandle) {
        lastError = "libcurl not initialized";
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

    // Enable SSL verification for security
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

    // Perform the request
    CURLcode res = curl_easy_perform(curl);

    // Check for errors
    if (res != CURLE_OK) {
        lastError = std::string("curl_easy_perform() failed: ") +
                   curl_easy_strerror(res);
        return "";
    }

    return readBuffer;
}

/**
 * @brief Extract a field from an item using regex
 * @param content The item content
 * @param regex The regex to use for extraction
 * @return The extracted field or empty string if not found
 */
static std::string extractField(const std::string& content, const std::regex& regex) {
    std::smatch match;
    if (std::regex_search(content, match, regex) && match.size() > 1) {
        return match.str(1);
    }
    return "";
}

std::vector<NewsItem> RSSFeed::parseRSS(const std::string& data, int count) {
    std::vector<NewsItem> items;

    // Simple regex-based parsing for demonstration purposes
    // In a real application, you would use a proper XML parser like libxml2

    // Define constants for item tags and regexes
    const std::string itemStart = "<item>";
    const std::string itemEnd = "</item>";
    const std::regex titleRegex("<title>(.*?)</title>");
    const std::regex linkRegex("<link>(.*?)</link>");
    const std::regex descRegex("<description>(.*?)</description>");
    const std::regex dateRegex("<pubDate>(.*?)</pubDate>");
    const std::regex guidRegex("<guid>(.*?)</guid>");
    const std::regex htmlTagRegex("<[^>]*>");

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

        // Create a news item and extract fields
        NewsItem item;
        item.title = extractField(itemContent, titleRegex);
        item.link = extractField(itemContent, linkRegex);
        item.pubDate = extractField(itemContent, dateRegex);
        item.guid = extractField(itemContent, guidRegex);

        // Extract and clean description
        item.description = extractField(itemContent, descRegex);
        if (!item.description.empty()) {
            // Clean up HTML tags from description
            item.description = std::regex_replace(item.description, htmlTagRegex, "");
        }

        // Add the item to the list
        items.push_back(item);
        itemCount++;

        // Call progress callback if set
        if (progressCallback) {
            progressCallback(itemCount, count);
        }
    }

    return items;
}
