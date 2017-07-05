#if PIXIE_IS_DEBUG_BUILD == $IS_DEBUG_BUILD$ && PIXIE_IS_PORTABLE_BUILD == $IS_PORTABLE_BUILD$

namespace engine
{
	class manifest_app_t
	{
		
	public:
		
		uint32_t get_version_major() const
		{
			return $VERSION_MAJOR$;
		}
		
		uint32_t get_version_minor() const
		{
			return $VERSION_MINOR$;
		}
		
		uint32_t get_version_revision() const
		{
			return $VERSION_REVISION$;
		}
		
		uint32_t get_version_build() const
		{
			return $VERSION_BUILD$;
		}
		
$MANIFEST_APP$

	};
}

#endif