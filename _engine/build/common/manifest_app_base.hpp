#if PIXIE_IS_DEBUG_BUILD == $IS_DEBUG_BUILD$ && PIXIE_IS_PORTABLE_BUILD == $IS_PORTABLE_BUILD$

namespace engine
{
	class manifest_app_t
	{
		
	public:
		
		static uint32_t get_version_major()
		{
			return $VERSION_MAJOR$;
		}
		
		static uint32_t get_version_minor()
		{
			return $VERSION_MINOR$;
		}
		
		static uint32_t get_version_revision()
		{
			return $VERSION_REVISION$;
		}
		
		static uint32_t get_version_build()
		{
			return $VERSION_BUILD$;
		}
		
$MANIFEST_APP$

	};
}

#endif