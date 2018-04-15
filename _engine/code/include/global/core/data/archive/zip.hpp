#ifndef ENGINE_COMMON_UTILITY_ARCHIVE_ZIP_HPP
#define ENGINE_COMMON_UTILITY_ARCHIVE_ZIP_HPP
#pragma once

#include <unzip.h>
#include <iostream>
#include "global/core/data/archive.hpp"

namespace engine
{
	namespace data
	{
		namespace archive
		{


			class zip_input_t : input_base_t
			{

			public:

				zip_input_t(std::unique_ptr<data::input_t> input, std::filesystem::file_time_type time_last_mod = std::filesystem::file_time_type::min()) : input_base_t(std::move(input), time_last_mod)
				{
					read_archive();
				}

				zip_input_t(data::provider_actual_t * provider) : input_base_t(provider)
				{
					read_archive();
				}

				std::unique_ptr<data::input_t> get_file(const ustring_t & archive_path) final;

			private:

				unzFile file;

				void read_archive();

				static void fill_filefunc(zlib_filefunc64_def_s * filefunc_def, zip_input_t * owner)
				{
					filefunc_def->opaque = owner;
					filefunc_def->zopen64_file = zip_zopen64_file;
					filefunc_def->zread_file = zip_zread_file;
					filefunc_def->zwrite_file = zip_zwrite_file;
					filefunc_def->ztell64_file = zip_ztell64_file;
					filefunc_def->zseek64_file = zip_zseek64_file;
					filefunc_def->zclose_file = zip_zclose_file;
					filefunc_def->zerror_file = zip_zerror_file;
				}

				static void * ZCALLBACK zip_zopen64_file(void * opaque, const void *filename, int mode)
				{
					return opaque;
				}

				static unsigned long ZCALLBACK zip_zread_file(void * opaque, void * stream, void * buf, unsigned long size)
				{
					zip_input_t * owner = static_cast<zip_input_t*>(stream);
					if (!owner || !owner->get_input()) return 0;

					return owner->get_input()->read(static_cast<uint8_t*>(buf), size);
				}

				static unsigned long ZCALLBACK zip_zwrite_file(void * opaque, void * stream, const void *buf, unsigned long size)
				{
					return 0;
				}

				static uint64_t ZCALLBACK zip_ztell64_file(void * opaque, void * stream)
				{
					zip_input_t * owner = static_cast<zip_input_t*>(stream);
					if (!owner || !owner->get_input()) return 0;

					return owner->get_input()->position();
				}

				static long ZCALLBACK zip_zseek64_file(void * opaque, void * stream, uint64_t offset, int origin)
				{
					zip_input_t * owner = static_cast<zip_input_t*>(stream);
					if (!owner || !owner->get_input()) return -1;
					
					switch (origin)
					{
					case ZLIB_FILEFUNC_SEEK_CUR:
						owner->get_input()->skip(offset);
						break;
					case ZLIB_FILEFUNC_SEEK_END:
						owner->get_input()->jump_to_end();
						owner->get_input()->go_back(offset);
						break;
					case ZLIB_FILEFUNC_SEEK_SET:
						owner->get_input()->jump_to_begin();
						owner->get_input()->skip(offset);
						break;
					}
					return 0;
				}

				static int  ZCALLBACK zip_zclose_file(void * opaque, void * stream)
				{
					return 0;
				}

				static int ZCALLBACK zip_zerror_file(void * opaque, void * stream)
				{
					return 0;
				}

				static int ZCALLBACK zip_comparision(unzFile file, const char *filename1, const char *filename2)
				{
					return stricmp(filename1, filename2);
				}

				std::recursive_mutex zip_access;
			};
		}

	}
}

#endif