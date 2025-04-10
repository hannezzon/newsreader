#ifndef RSS_FEED_HPP
#define RSS_FEED_HPP

#include <string>
#include <vector>
#include <memory>
#include <functional>

/**
 * @struct NewsItem
 * @brief Represents a single news item from an RSS feed
 */
struct NewsItem {
    std::string title;
    std::string link;
    std::string description;
    std::string pubDate;
    std::string guid;
};

/**
 * @class RSSFeed
 * @brief Fetches and parses RSS feeds
 */
class RSSFeed {
public:
    RSSFeed();
    ~RSSFeed();

    /**
     * @brief Fetch the latest news from the New York Times RSS feed
     * @param count The maximum number of news items to fetch
     * @return A vector of NewsItem objects
     */
    std::vector<NewsItem> fetchNews(int count = 10);

    /**
     * @brief Fetch the latest news from the AP RSS feed (legacy method name)
     * @param count The maximum number of news items to fetch
     * @return A vector of NewsItem objects
     */
    std::vector<NewsItem> fetchAPNews(int count = 10) { return fetchNews(count); }

    /**
     * @brief Set a callback function to be called during the fetch operation
     * @param callback The callback function
     */
    void setProgressCallback(std::function<void(int, int)> callback);

    /**
     * @brief Get the last error message
     * @return The last error message
     */
    std::string getLastError() const;

private:
    /**
     * @brief Fetch data from a URL
     * @param url The URL to fetch
     * @return The fetched data as a string
     */
    std::string fetchURL(const std::string& url);

    /**
     * @brief Parse RSS feed data
     * @param data The RSS feed data
     * @param count The maximum number of news items to parse
     * @return A vector of NewsItem objects
     */
    std::vector<NewsItem> parseRSS(const std::string& data, int count);

    // Callback function for progress updates
    std::function<void(int, int)> progressCallback;

    // Last error message
    std::string lastError;

    // Internal data for libcurl
    void* curlHandle;
};

#endif // RSS_FEED_HPP
