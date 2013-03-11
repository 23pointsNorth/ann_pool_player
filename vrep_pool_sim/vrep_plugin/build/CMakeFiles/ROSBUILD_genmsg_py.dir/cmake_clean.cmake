FILE(REMOVE_RECURSE
  "../msg_gen"
  "../srv_gen"
  "../msg_gen"
  "../srv_gen"
  "../src/vrep_plugin/msg"
  "../src/vrep_plugin/srv"
  "CMakeFiles/ROSBUILD_genmsg_py"
  "../src/vrep_plugin/msg/__init__.py"
  "../src/vrep_plugin/msg/_Turn.py"
  "../src/vrep_plugin/msg/_Pool_table_state.py"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/ROSBUILD_genmsg_py.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
