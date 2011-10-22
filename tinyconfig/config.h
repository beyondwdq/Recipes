#ifndef CONFIG_H
#define CONFIG_H

#include <map>
#include <string>
#include <cassert>

class Config{
	public:
		struct Item{
			inline Item(bool *pb, bool default_value)
				: p_bool_(pb),
				p_int_(NULL),
				p_double_(NULL)
			{
				*p_bool_ = default_value;
			}

			inline Item(int *pi, int default_value)
				: p_bool_(NULL),
				p_int_(pi),
				p_double_(NULL)
			{
				*p_int_ = default_value;
			}

			inline Item(double *pd, double default_value)
				: p_bool_(NULL),
				p_int_(NULL),
				p_double_(pd)
			{
				*p_double_ = default_value;
			}

			bool set(const std::string& value);

			bool *p_bool_;
			int  *p_int_;
			double *p_double_;
		};

		typedef std::map<std::string, Item> TItemMap;

		//constructor
		Config();
		virtual ~Config();

		bool load(const char *filename);
		bool set(const std::string& name, const std::string& value);

	protected:
		inline void add(const char *name, bool *pv, bool default_value)
		{
			assert(item_map_.find(std::string(name))==item_map_.end());
			item_map_.insert(TItemMap::value_type(
						std::string(name), Item(pv, default_value)));
		}

		inline void add(const char *name, int *pv, int default_value)
		{
			assert(item_map_.find(std::string(name))==item_map_.end());
			item_map_.insert(TItemMap::value_type(
						std::string(name), Item(pv, default_value)));
		}

		inline void add(const char *name, double *pv, double default_value)
		{
			assert(item_map_.find(std::string(name))==item_map_.end());
			item_map_.insert(TItemMap::value_type(
						std::string(name), Item(pv, default_value)));
		}


	private:
		TItemMap item_map_;


};
#endif //__CONFIG_H
