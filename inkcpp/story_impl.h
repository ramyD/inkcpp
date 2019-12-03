#pragma once

#include <system.h>
#include <config.h>
#include "types.h"
#include "story.h"

namespace ink::runtime::internal
{
	// Ink story. Constant once constructed. Can be shared safely between multiple runner instances
	class story_impl : public story
	{
	public:
#ifdef INK_ENABLE_STL
		story_impl(const char* filename);
#endif
		// Create story from allocated binary data in memory. If manage is true, this class will delete
		//  the pointers on destruction
		story_impl(unsigned char* binary, size_t len, bool manage = true);
		virtual ~story_impl();

		const char* string(uint32_t index) const;
		inline const ip_t instructions() const { return _instruction_data; }
		inline const ip_t end() const { return _file + _length; }

		inline uint32_t num_containers() const { return _num_containers; }

		bool iterate_containers(const uint32_t*& iterator, container_t& index, ip_t& offset, bool reverse = false) const;
		bool get_container_id(ip_t offset, container_t& container_id) const;

		// Creates a new global store for use with runners executing this story
		virtual globals new_globals() override;
		virtual runner new_runner(globals store = nullptr) override;

	private:
		void setup_pointers();

	private:
		// file information
		unsigned char* _file;
		size_t _length;

		// string table
		const char* _string_table;

		// container info
		uint32_t* _container_list;
		uint32_t _container_list_size;
		uint32_t _num_containers;

		// instruction info
		ip_t _instruction_data;

		// story block used to creat various weak pointers
		ref_block* _block;

		// whether we need to delete our binary data after we destruct
		bool _managed;
	};
}