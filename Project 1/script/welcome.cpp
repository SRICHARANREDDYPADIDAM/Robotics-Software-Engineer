#include <gazebo/gazebo.hh>

namespace gazebo
{
  class WorldPluginProjectOne : public WorldPlugin
  {
    public: WorldPluginProjectOne() : WorldPlugin()
            {
              printf("Welcome to Charan's World!\n");
            }

    public: void Load(physics::WorldPtr _world, sdf::ElementPtr _sdf) {}
  };
  GZ_REGISTER_WORLD_PLUGIN(WorldPluginProjectOne)
}
