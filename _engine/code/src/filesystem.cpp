#include "global/component/filesystem.hpp"

engine::filesystem_t::~filesystem_t()
{

}

engine::filesystem_t::file_output_t::~file_output_t()
{

}

engine::filesystem_t::file_input_t::~file_input_t()
{

}

void engine::filesystem_t::move_file(std::filesystem::path src, std::filesystem::path trg)
{
    copy_file(src, trg);
    delete_file(src);
}

void engine::filesystem_t::move_directory(std::filesystem::path src, std::filesystem::path trg)
{
    copy_directory(src, trg);
    delete_directory(src);
}

std::filesystem::file_time_type engine::filesystem_dummy_t::get_mod_time(std::filesystem::path path)
{
    return std::filesystem::file_time_type::min();
}

engine::paths_t engine::filesystem_dummy_t::iterate_files_in_directory(std::filesystem::path path, const ustring_t & pattern)
{
    return paths_t();
}
engine::paths_t engine::filesystem_dummy_t::iterate_directories(std::filesystem::path path, const ustring_t & pattern)
{
    return paths_t();
}
engine::paths_t engine::filesystem_dummy_t::iterate_files_in_subdirectories(std::filesystem::path path, const ustring_t & pattern)
{
    return paths_t();
}
engine::paths_t engine::filesystem_dummy_t::iterate_subdirectories(std::filesystem::path path, const ustring_t & pattern)
{
    return paths_t();
}

std::shared_ptr<engine::filesystem_t::file_output_t> engine::filesystem_dummy_t::output(std::filesystem::path path)
{
    return std::make_shared<file_output_dummy_t>();
}
std::shared_ptr<engine::filesystem_t::file_input_t> engine::filesystem_dummy_t::input(std::filesystem::path path)
{
    return std::make_shared<file_input_dummy_t>();
}

std::filesystem::path engine::filesystem_dummy_t::construct_temp_file()
{
    return std::filesystem::path();
}
bool engine::filesystem_dummy_t::exists(std::filesystem::path path)
{
    return false;
}
bool engine::filesystem_dummy_t::is_read_only(std::filesystem::path path)
{
    return true;
}
bool engine::filesystem_dummy_t::is_directory(std::filesystem::path path)
{
    return false;
}

void engine::filesystem_dummy_t::copy_file(std::filesystem::path src, std::filesystem::path trg)
{

}
void engine::filesystem_dummy_t::copy_directory(std::filesystem::path src, std::filesystem::path trg)
{

}

void engine::filesystem_dummy_t::delete_file(std::filesystem::path path)
{

}
void engine::filesystem_dummy_t::delete_directory(std::filesystem::path path)
{

}

engine::filesystem_dummy_t::file_output_dummy_t::file_output_dummy_t()
{

}

engine::filesystem_dummy_t::file_output_dummy_t::~file_output_dummy_t()
{

}

uint32_t engine::filesystem_dummy_t::file_output_dummy_t::write(const uint8_t * buffer, uint32_t size)
{
    return size;
}

void engine::filesystem_dummy_t::file_output_dummy_t::flush()
{

}

engine::filesystem_dummy_t::file_input_dummy_t::file_input_dummy_t()
{

}

engine::filesystem_dummy_t::file_input_dummy_t::~file_input_dummy_t()
{

}

void engine::filesystem_dummy_t::file_input_dummy_t::seek(int32_t position, file_seek_origin_t origin)
{

}

bool engine::filesystem_dummy_t::file_input_dummy_t::is_eof() const
{
    return true;
}

uint32_t engine::filesystem_dummy_t::file_input_dummy_t::tell() const
{
    return 0;
}

uint32_t engine::filesystem_dummy_t::file_input_dummy_t::read(uint8_t * buffer, uint32_t size)
{
    return 0;
}

engine::filesystem_provider_base_t::~filesystem_provider_base_t()
{

}

engine::paths_t engine::filesystem_provider_base_t::iterate_files_in_subdirectories(std::filesystem::path path, const ustring_t & pattern)
{
    paths_t ret = iterate_files_in_directory(path, pattern);

    for (auto dir : iterate_directories(path))
    {
        paths_t inside = iterate_files_in_subdirectories(path / dir, pattern);

        for (uint32_t i = 0; i < inside.size(); i++)
        {
            inside[i] = dir / inside[i];
        }

        ret.insert(ret.end(), inside.begin(), inside.end());
    }

    return ret;
}

engine::paths_t engine::filesystem_provider_base_t::iterate_subdirectories(std::filesystem::path path, const ustring_t & pattern)
{
    paths_t ret = iterate_directories(path, pattern);
    paths_t dirs = ret;

    for (auto dir : dirs)
    {
        paths_t inside = iterate_subdirectories(path / dir, pattern);

        for (uint32_t i = 0; i < inside.size(); i++)
        {
            inside[i] = dir / inside[i];
        }

        ret.insert(ret.end(), inside.begin(), inside.end());
    }

    return ret;
}

void engine::filesystem_provider_base_t::move_file(std::filesystem::path src, std::filesystem::path trg)
{
    copy_file(src, trg);
    delete_file(src);
}

void engine::filesystem_provider_base_t::move_directory(std::filesystem::path src, std::filesystem::path trg)
{
    copy_directory(src, trg);
    delete_directory(src);
}


bool engine::filesystem_provider_base_t::match(char const *needle, wchar_t const *haystack)
{
    for (; *needle != '\0'; ++needle)
    {
        switch (*needle)
        {
        case '?':
            if (*haystack == '\0')
                return false;
            ++haystack;
            break;
        case '*':
        {
            if (needle[1] == '\0')
                return true;
            size_t max = wcslen(haystack);
            for (size_t i = 0; i < max; i++)
                if (match(needle + 1, haystack + i))
                    return true;
            return false;
        }
        default:
            if (*haystack != *needle)
                return false;
            ++haystack;
        }
    }
    return *haystack == '\0';
}
bool engine::filesystem_provider_base_t::match(char const *needle, char const *haystack)
{
    for (; *needle != '\0'; ++needle)
    {
        switch (*needle)
        {
        case '?':
            if (*haystack == '\0')
                return false;
            ++haystack;
            break;
        case '*':
        {
            if (needle[1] == '\0')
                return true;
            size_t max = strlen(haystack);
            for (size_t i = 0; i < max; i++)
                if (match(needle + 1, haystack + i))
                    return true;
            return false;
        }
        default:
            if (*haystack != *needle)
                return false;
            ++haystack;
        }
    }
    return *haystack == '\0';
}

engine::filesystem_real_t::filesystem_real_t(std::unique_ptr<holder_t<filesystem_t> > filesystem_provider) : filesystem_provider(std::move(filesystem_provider))
{

}

std::filesystem::file_time_type engine::filesystem_real_t::get_mod_time(std::filesystem::path path)
{
    return filesystem_provider->get_provider()->get_mod_time(path);
}

engine::paths_t engine::filesystem_real_t::iterate_files_in_directory(std::filesystem::path path, const ustring_t & pattern)
{
    return filesystem_provider->get_provider()->iterate_files_in_directory(path, pattern);
}
engine::paths_t engine::filesystem_real_t::iterate_directories(std::filesystem::path path, const ustring_t & pattern)
{
    return filesystem_provider->get_provider()->iterate_directories(path, pattern);
}
engine::paths_t engine::filesystem_real_t::iterate_files_in_subdirectories(std::filesystem::path path, const ustring_t & pattern)
{
    return filesystem_provider->get_provider()->iterate_files_in_subdirectories(path, pattern);
}
engine::paths_t engine::filesystem_real_t::iterate_subdirectories(std::filesystem::path path, const ustring_t & pattern)
{
    return filesystem_provider->get_provider()->iterate_subdirectories(path, pattern);
}

std::shared_ptr<engine::filesystem_t::file_output_t> engine::filesystem_real_t::output(std::filesystem::path path)
{
    return filesystem_provider->get_provider()->output(path);
}
std::shared_ptr<engine::filesystem_t::file_input_t> engine::filesystem_real_t::input(std::filesystem::path path)
{
    return filesystem_provider->get_provider()->input(path);
}

std::filesystem::path engine::filesystem_real_t::construct_temp_file()
{
    return filesystem_provider->get_provider()->construct_temp_file();
}
bool engine::filesystem_real_t::exists(std::filesystem::path path)
{
    return filesystem_provider->get_provider()->exists(path);
}
bool engine::filesystem_real_t::is_read_only(std::filesystem::path path)
{
    return filesystem_provider->get_provider()->is_read_only(path);
}
bool engine::filesystem_real_t::is_directory(std::filesystem::path path)
{
    return filesystem_provider->get_provider()->is_directory(path);
}

void engine::filesystem_real_t::copy_file(std::filesystem::path src, std::filesystem::path trg)
{
    filesystem_provider->get_provider()->copy_file(src, trg);
}
void engine::filesystem_real_t::copy_directory(std::filesystem::path src, std::filesystem::path trg)
{
    filesystem_provider->get_provider()->copy_directory(src, trg);
}

void engine::filesystem_real_t::delete_file(std::filesystem::path path)
{
    filesystem_provider->get_provider()->delete_file(path);
}
void engine::filesystem_real_t::delete_directory(std::filesystem::path path)
{
    filesystem_provider->get_provider()->delete_directory(path);
}
void engine::filesystem_real_t::move_file(std::filesystem::path src, std::filesystem::path trg)
{
    filesystem_provider->get_provider()->move_file(src, trg);
}
void engine::filesystem_real_t::move_directory(std::filesystem::path src, std::filesystem::path trg)
{
    filesystem_provider->get_provider()->move_directory(src, trg);
}

engine::filesystem_provider_generic_t::file_output_generic_t::file_output_generic_t(std::filesystem::path physical_path) : physical_path(physical_path)
{
    std::error_code ec;

    std::filesystem::path dir = physical_path;
    dir.remove_filename();
    if (!std::filesystem::is_directory(dir, ec))
        create_directories(dir, ec);

    file.open(physical_path, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
}

engine::filesystem_provider_generic_t::file_output_generic_t::~file_output_generic_t()
{
    file.close();
}

uint32_t engine::filesystem_provider_generic_t::file_output_generic_t::write(const uint8_t * buffer, uint32_t size)
{
    file.write(reinterpret_cast<const char*>(buffer), size);
    return size;
}

void engine::filesystem_provider_generic_t::file_output_generic_t::flush()
{
    file.flush();
}

engine::filesystem_provider_generic_t::file_input_generic_t::file_input_generic_t(std::filesystem::path path)
{
    file.open(path, std::ios_base::in | std::ios_base::binary);
}

engine::filesystem_provider_generic_t::file_input_generic_t::~file_input_generic_t()
{
    file.close();
}

void engine::filesystem_provider_generic_t::file_input_generic_t::seek(int32_t position, file_seek_origin_t origin)
{
    switch (origin)
    {
    case file_seek_origin_t::begin: file.seekg(position, std::ios_base::beg); break;
    case file_seek_origin_t::current: file.seekg(position, std::ios_base::cur); break;
    case file_seek_origin_t::end: file.seekg(position, std::ios_base::end); break;
    }
}
bool engine::filesystem_provider_generic_t::file_input_generic_t::is_eof() const
{
    return file.eof();
}
uint32_t engine::filesystem_provider_generic_t::file_input_generic_t::tell() const
{
    return file.tellg();
}
uint32_t engine::filesystem_provider_generic_t::file_input_generic_t::read(uint8_t * buffer, uint32_t size)
{
    file.read(reinterpret_cast<char *>(buffer), size);

    return size;
}

engine::filesystem_provider_generic_t::filesystem_provider_generic_t() : tmp_item(0)
{

}

std::filesystem::file_time_type engine::filesystem_provider_generic_t::get_mod_time(std::filesystem::path path)
{
    std::error_code ec;
    return std::filesystem::last_write_time(path, ec);
}

engine::paths_t engine::filesystem_provider_generic_t::iterate_files_in_directory(std::filesystem::path path, const ustring_t & pattern)
{
    paths_t ret;
    std::error_code ec;

    for (auto & item : std::filesystem::directory_iterator(path, ec))
    {
        if (std::filesystem::is_regular_file(item.status()))
        {
            std::string str = item.path().filename().string();
            if (match(pattern.get_cstring(), str.c_str()))
            {
                ret.push_back(item.path().filename());
            }
        }
    }

    return ret;
}
engine::paths_t engine::filesystem_provider_generic_t::iterate_directories(std::filesystem::path path, const ustring_t & pattern)
{
    paths_t ret;
    std::error_code ec;

    for (auto & item : std::filesystem::directory_iterator(path, ec))
    {
        if (std::filesystem::is_directory(item.status()))
        {
            std::string str = item.path().filename().string();
            if (match(pattern.get_cstring(), str.c_str()))
            {
                ret.push_back(item.path().filename());
            }
        }
    }

    return ret;
}
engine::paths_t engine::filesystem_provider_generic_t::iterate_files_in_subdirectories(std::filesystem::path path, const ustring_t & pattern)
{
    paths_t ret;
    std::error_code ec;

    for (auto & item : std::filesystem::recursive_directory_iterator(path, ec))
    {
        if (std::filesystem::is_regular_file(item.status()))
        {
            std::string str = item.path().filename().string();
            if (match(pattern.get_cstring(), str.c_str()))
            {
                ret.push_back(naive_uncomplete(item.path(), path));
            }
        }
    }

    return ret;
}
engine::paths_t engine::filesystem_provider_generic_t::iterate_subdirectories(std::filesystem::path path, const ustring_t & pattern)
{
    paths_t ret;
    std::error_code ec;

    for (auto & item : std::filesystem::recursive_directory_iterator(path, ec))
    {
        if (std::filesystem::is_directory(item.status()))
        {
            std::string str = item.path().filename().string();
            if (match(pattern.get_cstring(), str.c_str()))
            {
                ret.push_back(naive_uncomplete(item.path(), path));
            }
        }
    }

    return ret;
}

std::shared_ptr<engine::filesystem_t::file_output_t> engine::filesystem_provider_generic_t::output(std::filesystem::path path)
{
    return std::make_shared<file_output_generic_t>(path);
}

std::shared_ptr<engine::filesystem_t::file_input_t> engine::filesystem_provider_generic_t::input(std::filesystem::path path)
{
    return std::make_shared<file_input_generic_t>(path);
}

std::filesystem::path engine::filesystem_provider_generic_t::construct_temp_file()
{
    std::lock_guard<std::mutex> guard(tmp_mutex);

    std::error_code ec;
    std::filesystem::path path = std::filesystem::temp_directory_path(ec);
    std::filesystem::path tmp_file;

    do
    {

        tmp_file = path;

        ustring_t filename = format_string("#1#_#2#.tmp"_u, ustring_t::from_utf8(PIXIE_PROJECT_NAME), ++tmp_item);

        tmp_file /= filename.get_cstring();

    } while (std::filesystem::exists(tmp_file));

    std::ofstream file;
    file.open(tmp_file, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
    file.close();
    
    return tmp_file;
}

bool engine::filesystem_provider_generic_t::exists(std::filesystem::path path)
{
    std::error_code ec;
    return std::filesystem::exists(path, ec);
}

bool engine::filesystem_provider_generic_t::is_read_only(std::filesystem::path path)
{
    return false;
}

bool engine::filesystem_provider_generic_t::is_directory(std::filesystem::path path)
{
    std::error_code ec;
    return std::filesystem::is_directory(path, ec);
}

void engine::filesystem_provider_generic_t::copy_file(std::filesystem::path src, std::filesystem::path trg)
{
    std::error_code ec;
    std::filesystem::copy_file(src, trg, std::filesystem::copy_options::overwrite_existing, ec);
}

void engine::filesystem_provider_generic_t::copy_directory(std::filesystem::path src, std::filesystem::path trg)
{
    std::error_code ec;
    std::filesystem::copy(src, trg, std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive, ec);
}

void engine::filesystem_provider_generic_t::delete_file(std::filesystem::path path)
{
    std::error_code ec;
    std::filesystem::remove(path, ec);
}

void engine::filesystem_provider_generic_t::delete_directory(std::filesystem::path path)
{
    std::error_code ec;
    std::filesystem::remove_all(path, ec);
}

std::filesystem::path engine::filesystem_provider_generic_t::naive_uncomplete(std::filesystem::path const p, std::filesystem::path const base)
{

    using std::filesystem::path;

    if (p == base)
        return "./";
    /*!! this breaks stuff if path is a filename rather than a directory,
    which it most likely is... but then base shouldn't be a filename so... */

    std::filesystem::path from_path, from_base, output;

    std::filesystem::path::iterator path_it = p.begin(), path_end = p.end();
    std::filesystem::path::iterator base_it = base.begin(), base_end = base.end();

    // check for emptiness
    if ((path_it == path_end) || (base_it == base_end))
        throw std::runtime_error("path or base was empty; couldn't generate relative path");

#ifdef WIN32
    // drive letters are different; don't generate a relative path
    if (*path_it != *base_it)
        return p;

    // now advance past drive letters; relative paths should only go up
    // to the root of the drive and not past it
    ++path_it, ++base_it;
#endif

    // Cache system-dependent dot, double-dot and slash strings
    const std::string _dot = ".";
    const std::string _dots = "..";
    const std::string _sep = "/";

    // iterate over path and base
    while (true) {

        // compare all elements so far of path and base to find greatest common root;
        // when elements of path and base differ, or run out:
        if ((path_it == path_end) || (base_it == base_end) || (*path_it != *base_it)) {

            // write to output, ../ times the number of remaining elements in base;
            // this is how far we've had to come down the tree from base to get to the common root
            for (; base_it != base_end; ++base_it) {
                if (*base_it == _dot)
                    continue;
                else if (*base_it == _sep)
                    continue;

                output /= "../";
            }

            // write to output, the remaining elements in path;
            // this is the path relative from the common root
            std::filesystem::path::iterator path_it_start = path_it;
            for (; path_it != path_end; ++path_it) {

                if (path_it != path_it_start)
                    output /= "/";

                if (*path_it == _dot)
                    continue;
                if (*path_it == _sep)
                    continue;

                output /= *path_it;
            }

            break;
        }

        // add directory level to both paths and continue iteration
        from_path /= path(*path_it);
        from_base /= path(*base_it);

        ++path_it, ++base_it;
    }

    return output;
}