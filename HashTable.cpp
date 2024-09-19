#include "HashTable.h"

//@params key-key from hash ,index- on which bucket to find the key
//return the vector in this current key
const std::vector<int> HashTable::getBucketFromKey(BucketKey key, int index)
{
    if (hash_table[index].find(key) != hash_table[index].end())
        return hash_table[index][key];
    return std::vector<int>();
}

//build mask for quick search in table
inline std::vector<size_t> HashTable::generateRandomMask(size_t feature_size, unsigned int key_size)
{
    size_t mask_size = (feature_size * CHAR_BIT + sizeof(size_t) - 1) / sizeof(size_t);

    std::vector<size_t> mask(mask_size, 0);
    // Create random indices
    std::vector<int> indices(feature_size * CHAR_BIT);
    for (size_t i = 0; i < feature_size * CHAR_BIT; ++i) indices[i] = (int)i;
    
// ערבוב הוקטור עם std::shuffle

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(indices.begin(), indices.end(),g);
   
    // Set the appropriate bits in the mask
    for (unsigned int i = 0; i < key_size; ++i) {
        size_t index = indices[i];
        size_t divisor = CHAR_BIT * sizeof(size_t);
        size_t idx = index / divisor;
        mask[idx] |= size_t(1) << (index % divisor);
    }

    return mask;
}
void HashTable::fill_xor_mask(int key, int lowest_index, unsigned int level, std::vector<size_t>& xor_masks)
{
    xor_masks.push_back(key);
    if (level == 0) return;
    for (int index = lowest_index - 1; index >= 0; --index) {
        // Create a new key
        int new_key = key | (1 << index);
        fill_xor_mask(new_key, index, level - 1, xor_masks);
    }

}
size_t HashTable::getKey(std::vector<int> descrip)
{
    // Use iterators instead of reinterpret cast
    auto feature_block_it = descrip.begin();
    std::vector<size_t> mask = generateRandomMask(32, 10);

    // Initialize the key components
    size_t subsignature = 0;
    size_t bit_index = 1;
    unsigned int feature_size_ = 32;
    size_t mask_size = mask.size();

    for (unsigned i = 0; i < feature_size_; i += sizeof(size_t)) {
        // Ensure we do not exceed the buffer size
        if (i + sizeof(size_t) > feature_size_) {
            break;
        }

        // Check for remaining bytes at the end of the feature
        if (i > feature_size_ - sizeof(size_t)) {
            size_t tmp = 0;
            std::copy(feature_block_it, feature_block_it + (feature_size_ - i), &tmp); // Preserve byte order
            feature_block_it = descrip.end(); // No further processing needed
        }

        // Ensure mask index is within bounds
        size_t mask_index = i / sizeof(size_t);
        if (mask_index >= mask_size) {
            break;
        }

        // Get the corresponding mask block
        size_t mask_block = mask[mask_index];

        while (mask_block) {
            // Get the lowest set bit in the mask block
            size_t lowest_bit = mask_block & ~(mask_block - 1);

            // Add it to the current subsignature if necessary
            subsignature += (*feature_block_it & lowest_bit) ? bit_index : 0;

            // Reset the bit in the mask block
            mask_block ^= lowest_bit;

            // Increment the bit index for the subsignature
            bit_index <<= 1;

            // Move to the next element (if not already at the end)
            if (feature_block_it != descrip.end()) {
                ++feature_block_it;
            }
        }
    }
    return subsignature;
}

//@params the dist between 2 key point ,the index of the train index

void ResultSet::addPoint(float distance, int index)
{
    //add point to result only if distance less then worst distance and sorts for next time.
    auto it = std::find_if(results.begin(), results.end(), [distance](std::pair<float, int>& val) {
        return val.first == distance; });
    if (it == results.end()) {
        if (results.size() < capacity)
            results.push_back(std::make_pair(distance, index));
        else
        {
            if (results[results.size() - 1].first > distance)
                results[results.size() - 1] = std::make_pair(distance, index);
        }

        std::sort(results.begin(), results.end(), [](const std::pair<float, int>& a, const std::pair<float, int>& b) {
            return a.first < b.first;
            });
        worst_dist = results.back().first;
    }
}

std::vector<int> ResultSet::getIndices()
{
    {
        std::vector<int> indices;
        for (const auto& result : results) {
            indices.push_back(result.second);
        }
        return indices;
    }
}

std::vector<float> ResultSet::getDistances()
{
    {
        std::vector<float> distances;
        for (const auto& result : results) {
            distances.push_back(result.first);
        }
        return distances;
    }
}
