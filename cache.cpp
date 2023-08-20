//////////////////////////////////////////////////////////////////////
// CPU CACHE SIMULATOR, PROJECT #1
//////////////////////////////////////////////////////////////////////

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <deque>

using namespace std;

//////////////////////////////////////////////////////////////////////
// TO MAKE INPUT MATCH EXACTLY AS PROJECT REQUIREMENT, SET DEBUG = 0
//////////////////////////////////////////////////////////////////////

#define DEBUG 0
#define LRU 0
#define FIFO 1
#define wt 0
#define wb 1

class Block
{
    public:
        // When a write miss occurs and using writeback turn this on.
        // Will always be turned on in the set if there is data in a block.
        int dirty_bit;

        // Initialized to 0 unless there is a write (tag or data in block).
        // Indicates that there is a piece of data within the block.
        int valid_bit;

        // The position it is in the stack or queue within the set.
        int pos_num;

        // The hex address converted to long long int.
        long long int tag;
};

class Cache
{
    public:
        // Used to configure the cache. These values never change once initialized.
        int CACHE_SIZE;
        int ASSOC;
        int REPLACEMENT;
        int WB;

        // Used to keep statistics of cache accesses.
        int num_hits;
        int num_misses;
        int num_write;
        int num_read;

        // Used for indexing and creating the 2D vector.
        int num_sets;

        // Create the 2D array.
        // Number of columns is equivalent to the associativity input.
        // Number of rows is the number of sets calculated above.
        vector<deque<Block> > cache;
        vector<int> num_valid_blocks;

        // cache[num_set].size() keeps capacity of each set.
        // if cache[num_set].size() == assoc, you can not add anymore and must evict lowest position or highest position.

        // Sets the cache configuration.
        Cache(string CACHE_SIZE, string ASSOC, string REPLACEMENT, string WB)
        {
            // Reading literal cache size value, so input 32768 instead of 32KB.
            this->CACHE_SIZE = stoi(CACHE_SIZE);
            this->ASSOC = stoi(ASSOC);

            if (REPLACEMENT == "0")
            {
                // LRU / Stack
                this->REPLACEMENT = 0;
            }
            else
            {
                // FIFO / Queue
                this->REPLACEMENT = 1;
            }

            if (WB == "0")
            {
                this->WB = 0;
            }
            else
            {
                this->WB = 1;
            }

            this->num_hits = 0;
            this->num_misses = 0;
            this->num_write = 0;
            this->num_read = 0;

            // Create the number of sets for the cache.
            // 2^10 is size of KB in bytes, so 1 2 4 8 16 32 64 would all be valid.
            // 2^6 is the block size, so every block is 64 bytes for all configurations.
            this->num_sets = ((this->CACHE_SIZE) / (this->ASSOC * 64));

            deque<Block> set;
            // There is 0 valid blocks for the whole cache.
            for (int i = 0; i < this->num_sets; i++)
            {
                Block temp;
                temp.valid_bit = -1;
                temp.dirty_bit = -1;
                temp.pos_num = -1;
                temp.tag = -1;

                // Push a single initializer block into the set deque.
                set.push_back(temp);

                // Push the set deque a cache set index.
                cache.push_back(set);
                
                num_valid_blocks.push_back(0);
                set.clear();
            }
        }
};

void print_cache_config(Cache &c)
{
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    cout << "CACHE CONFIG: " << endl;
    cout << "CACHE_SIZE: " << c.CACHE_SIZE << endl;
    cout << "ASSOC: " << c.ASSOC << endl;
    cout << "REPLACEMENT TYPE: " << c.REPLACEMENT << endl;
    cout << "WB TYPE: " << c.WB << endl;
    cout << "NUM_HITS: " << c.num_hits << endl;
    cout << "NUM_MISSES: " << c.num_misses << endl;
    cout << "NUM_WRITE: " << c.num_write << endl;
    cout << "NUM_READ: " << c.num_read << endl;
    cout << "NUM_SETS: " << c.num_sets << endl;
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
}

// Print a 2d vector of all the sets held within the cache.
void printCacheContents(Cache &c)
{
    for (int i = 0; i < c.cache.size(); i++)
    {
        cout << "Set: " << i << endl;
        for (auto itr = c.cache[i].begin(); itr != c.cache[i].end(); itr++)
        {
            cout << "set_number: " << i << " cache.tag: " << itr->tag << " cache.valid_bit: " << itr->valid_bit << " cache.pos_num: " << itr->pos_num << " cache.dirty_bit: " << itr->dirty_bit << endl;
        }
    }

    return;
}

// Convert a hex string into long long.
long long int hexStringToLongLong(const string& hexString)
{
    stringstream converter;
    converter << hex << hexString;
    long long int result;
    converter >> result;
    return result;
}

// Insert a new block into the set.
// This will activate only when the cache set size is 1 (and not default) or when size() < c.ASSOC.
// If cache set is full, it must be evicted using evict().
void new_block(Cache &c, long long int tag, long long int setIdx, string type)
{
    Block temp;
    temp.pos_num = c.cache[setIdx].size();
    temp.tag = tag;
    temp.valid_bit = 1;

    if (type == "write")
    {
        temp.dirty_bit = 1;
    }
    else // read
    {
        temp.dirty_bit = 0;
    }

    // Insert the newest block at the front of the cache.
    // Front of the cache is most recently used.
    c.cache[setIdx].push_front(temp);
    c.num_valid_blocks[setIdx]++;
}

// This will only be activated if a cache set is size() == c.ASSOC.
// Make an object MRU or LRU.
// If dirty bit == 1 && c.WB == 1, increment write.
// If c.WB == 0, increment write every time.
void evict(Cache &c, long long int tag, long long int setIdx, string type, string miss)
{
    // Evict the least recently used element (back of the deque).
    if (c.REPLACEMENT == LRU)
    {
        // Remove the back of the deque.

        // For last element.
        // If dirty bit is on, and write-back type, increment write count.
        if (c.cache[setIdx][c.cache[setIdx].size()-1].dirty_bit == 1 && c.WB == wb)
        {
            c.num_write++;
        }
        
        // Check if hit or miss type.

        // If miss, insert a new block to front of the deque and remove the old block.
        if (miss == "miss")
        {
            // Remove old element.
            c.cache[setIdx].pop_back();

            // Add the the front of the deque.
            new_block(c, tag, setIdx, type);
        }
        else // If hit, remove the old block and insert the same one at front.
        {
            // Form the new memory block.
            Block temp;
            temp.pos_num = 0;
            temp.valid_bit = 1;
            temp.tag = tag;

            // If write type, and old block has dirty bit off, turn dirty bit on.
            if (type == "write" && c.cache[setIdx][c.cache[setIdx].size()-1].dirty_bit == 0)
            {
                temp.dirty_bit = 1;
            }
            else
            {
                temp.dirty_bit = 0;
            }

            // Insert the new memory block into the set.
            c.cache[setIdx].push_front(temp);

            // Remove the old block from the set.
            c.cache[setIdx].pop_back();
        }
    }
    else // Evict the earliest element (front of the deque).
    {
        // Remove the front of the deque.

        // For first element.
        // If dirty bit is on, and write-back type, increment write count.
        if (c.cache[setIdx][0].dirty_bit == 1 && c.WB == wb)
        {
            c.num_write++;
        }

        // Check if hit or miss type.

        // If miss, remove the most recently used block (index 0)
        // and insert to index (0) once old block is evicted.
        if (miss == "miss")
        {
            // Remove most recently used element.
            c.cache[setIdx].pop_front();

            // Add to front of the deque.
            new_block(c, tag, setIdx, type);
        }
        else // If hit, remove the newest block and insert the same one at front.
        {
            // Form the new memory block.
            Block temp;
            temp.pos_num = 0;
            temp.valid_bit = 1;
            temp.tag = tag;

            // If write type, and old block has dirty bit off, turn dirty bit on.
            if (type == "write" && c.cache[setIdx][c.cache[setIdx].size()-1].dirty_bit == 0)
            {
                temp.dirty_bit = 1;
            }
            else
            {
                temp.dirty_bit = 0;
            }

            // Remove the newest block from the front set.
            c.cache[setIdx].pop_front();

            // Insert the new memory block into the set.
            c.cache[setIdx].push_front(temp);
        }
    }
}

// Make the position itr the most recently used item.
void update(Cache &c, long long int tag, long long int setIdx, string type)
{
    // Always increment when we are rewriting a block in write-through.
    if (c.WB == wt)
    {
        c.num_write++;
    }

    // Check for default value.
    if (c.cache[setIdx].size() == 1)
    {
        // Only the default block will be initialized to all negative values.
        // Replace values with updated information.
        if (c.cache[setIdx][0].tag == -1)
        {
            // Replace the existing block.
            c.cache[setIdx][0].valid_bit = 1;
            c.cache[setIdx][0].pos_num = 0;
            c.cache[setIdx][0].tag = tag;

            if (type == "write")
            {
                c.cache[setIdx][0].dirty_bit = 1;
            }
            else
            {
                c.cache[setIdx][0].dirty_bit = 0;
            }

            c.num_valid_blocks[setIdx]++;
        }
        else // There is a valid block in the set.
        {
            // Insert a new block into cache set.
            new_block(c, tag, setIdx, type);
        }
    }
    else // Size is bigger than 1.
    {
        // Cache is full.
        if (c.cache[setIdx].size() >= c.ASSOC)
        {
            string miss = "miss";
            evict(c, tag, setIdx, type, miss);
        }
        else // Cache set still contains space.
        {
            new_block(c, tag, setIdx, type);
        }
    }
}

// This will only be activated if the tag is already in a set.
// This will never evict a piece of data, only make the element MRU.
// This will change the dirty bit.
void replace(Cache &c, deque<Block>::iterator itr, long long int tag, long long int setIdx, string type)
{
    // Make a temp block with the hit block's data.
    // Make it position MRU.
    Block temp;
    temp.pos_num = 0;
    temp.tag = tag;
    temp.valid_bit = 1;

    // Turn dirty bit on if we are writing.
    if (itr->dirty_bit == 0 && type == "write")
    {
        temp.dirty_bit = 1;
    }
    else
    {
        temp.dirty_bit = 0;
    }

    // Remove the old block.
    c.cache[setIdx].erase(itr);

    // Move the old block into the MRU (index 0).
    c.cache[setIdx].push_front(temp);
}

// Make an item the most recently used.
// Item has been found in the set.
// If dirty bit is off, turn to on.
// mem_read++ when an item is evicted from the set and dirty bit on.
// If an item is evicted && dirty bit == on && wb == 1, increment memory write.
// If wb == 0, increment memory write every time.
void hit(Cache &c, deque<Block>::iterator itr, long long int tag, long long int setIdx, string type)
{
    c.num_hits++;

    // Replace the cache (because item is already in set).
    // If in correct place, leave it alone, if not then switch spots with front.
    replace(c, itr, tag, setIdx, type);
}

// Item has not been found in the cache and must be added.
// Insert new mem block into set as MRU.
// If write - dirty bit on, if read - dirty bit off.
// If an item is evicted && dirty bit == on && wb == 1, increment memory write.
// If wb == 0, increment memory write every time.
void miss(Cache &c, long long int tag, long long int setIdx, string type)
{
    c.num_misses++;
    c.num_read++;

    // Update the cache.
    update(c, tag, setIdx, type);
}

// Read or write to the address into the block of the corresponding set in the cache.
void access_cache(Cache &c, string operation, string address)
{
    // Convert the string address to a long long int address tag.
    // This is what goes inside the blocks inside a set.
    long long int tag = hexStringToLongLong(address);
    long long int block_size = 64;
    long long int setIdx = (tag / block_size) % c.num_sets;

    if (operation == "R") // READ
    {
        if (DEBUG == 1)
        {
            // print_cache_config(c);
            // printCacheContents(c);
            cout << "READING " << address << " TO CACHE SET INDEX " << setIdx << endl;
        }

        // Loop through whole cache to see if the tag is in the corresponding set.
        for (auto itr = c.cache[setIdx].begin(); itr != c.cache[setIdx].end(); itr++)
        {
            // Tag is found. Cache hit.
            if (itr->tag == tag)
            {
                hit(c, itr, tag, setIdx, "read");

                if (DEBUG == 1)
                {
                    cout << "TAG READ HIT" << endl;
                    print_cache_config(c);
                    // printCacheContents(c);
                }

                return;
            }
        }

        miss(c, tag, setIdx, "read");

        if (DEBUG == 1)
        {
            cout << "TAG READ MISS" << endl;
            print_cache_config(c);
            // printCacheContents(c);
        }

        return;
    }
    else if (operation == "W") // WRITE
    {
        if (DEBUG == 1)
        {
            // print_cache_config(c);
            // printCacheContents(c);
            cout << "WRITING " << address << " TO CACHE SET INDEX " << setIdx << endl;
        }

        // Always increment when we are rewriting a block in write-through.
        if (c.WB == wt)
        {
            c.num_write++;
        }

        for (auto itr = c.cache[setIdx].begin(); itr != c.cache[setIdx].end(); itr++)
        {
            // Tag is found. Cache hit.
            if (itr->tag == tag)
            {
                hit(c, itr, tag, setIdx, "write");

                if (DEBUG == 1)
                {
                    cout << "TAG WRITE HIT" << endl;
                    print_cache_config(c);
                    // printCacheContents(c);
                }

                return;
            }
        }

        miss(c, tag, setIdx, "write");

        if (DEBUG == 1)
        {
            cout << "TAG WRITE MISS" << endl;
            print_cache_config(c);
            // printCacheContents(c);
        }

        return;
    }
    else
    {
        if (DEBUG == 1)
        {
            cout << "UNKNOWN CACHE OPERATION. CANCELING CACHE SIMULATION AND ENDING PROGRAM." << endl;
        }
    }

    return;
}

int main(int argc, char **argv)
{
    if (argc != 6)
    {
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        cout << "Improper syntax, use: ./SIM <CACHE_SIZE> <ASSOC> <REPLACEMENT> <WB> <TRACE_FILE>" << endl;
        cout << "Example: ./SIM 32768 8 1 1 /home/TRACES/MCF.t" << endl;
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        return 1;
    }

    string CACHE_SIZE = argv[1]; // size of cache in bytes
    string ASSOC = argv[2]; // associativity
    string REPLACEMENT = argv[3]; // replacement policy: 0 = LRU, 1 = FIFO
    string WB = argv[4]; // Write back policy: 0 = write-through, 1 = write-back
    string TRACE_FILE = argv[5]; // trace file name directory

    // Input example:
    // ./SIM 32768 8 1 1 /home/TRACES/MCF.t simulates:
    // 32KB write back cache with 8 way associativity and FIFO
    // replacement policy. The memory trace will be read from
    // /home/TRACES/MCF.t

    // Create the cache object with argument parameters before accessing it using the given arguments.
    Cache cache(CACHE_SIZE, ASSOC, REPLACEMENT, WB);

    // Switch debug to 1 to check input parameters.
    if (DEBUG == 1)
    {
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        cout << "Arguments passed:" << endl;
        cout << CACHE_SIZE << " cache size" << endl;
        cout << ASSOC << " way associativity" << endl;

        if (REPLACEMENT == "0")
        {
            cout << REPLACEMENT << " LRU (LIFO/Stack) replacement policy" << endl;
        }
        else if (REPLACEMENT == "1")
        {
            cout << REPLACEMENT << " FIFO/Queue replacement policy" << endl;
        }
        else
        {
            cout << REPLACEMENT << " unknown replacement policy" << endl;
        }

        if (WB == "0")
        {
            cout << WB << " write-through write-back policy (cache to memory every time you write to cache)" << endl;
        }
        else if (WB == "1")
        {
            cout << WB << " write-back write-back policy (cache to memory if data popped from cache is dirty)" << endl;
        }
        else
        {
           cout << WB << " unknown write-back policy" << endl;
        }

        cout << "TRACE FILE: (" << TRACE_FILE << ")" << endl;
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    }

    // Check to make sure that cache is created properly.
    if (DEBUG == 1)
    {
        cout << "CACHE CONTENTS AT BEGIN OF FILE INPUT" << endl;
        print_cache_config(cache);
        // printCacheContents(cache);
    }

    // Reading from the file.
    if (DEBUG == 1)
    {
        cout << "OPERATION TYPE:\t\t\tADDRESS:" << endl;
    }
    ifstream infile(argv[5]);
    string line;

    // Keeps track of the read and write count.
    int count = 1;

    while (getline(infile, line))
    {
        istringstream iss(line);
        string type, address;
        if (!(iss >> type >> address)) { break; }
        
        if (DEBUG == 1)
        {
            cout << "Count: " << count++ << endl;
            cout << type << "\t\t\t\t" << address << endl;
        }

        // Read or write to cache using the inputs and process each request.
        // Type and address are strings.
        access_cache(cache, type, address);

        if (DEBUG == 1)
        {
            cout << endl;
        }
    }

    if (DEBUG == 1)
    {
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        cout << "CACHE CONFIG AT END OF FILE INPUT" << endl;
        print_cache_config(cache);
        printCacheContents(cache);
    }

    // Hit ratio = hits / memory accesses (memory read or write).
    // Miss ratio = misses / memory accesses (memory read or write or 1 - hit ratio).
    float miss_ratio = ((float)cache.num_misses / ((float)cache.num_write + (float)cache.num_read));
    cout << "MISS RATIO: " << miss_ratio << endl;
    cout << "# WRITES TO MEMORY: " << cache.num_write << endl;
    cout << "# READS FROM MEMORY: " << cache.num_read << endl;

    // Close out the file once done reading.
    infile.close();

    return 0;
}
