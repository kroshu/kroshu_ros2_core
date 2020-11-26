# Core classes which help function the repositories of kroshu.
These classes provide functonalities which are frequently used in ROS2 environment.
Deriving from these classes the user has a helpful wrapper around the base functionalities of ROS2.

Right now the only class implemented in this repository is the ROS2BaseNode class. 
It furthers the rclcpp_lifecycle::LifecycleNode class by implementing lifecycle functions which would be usually implemented in the same way in every case.
But they are virtual functions, so it is possible to override them in the case of a different desired implementation.

Furthermore the class provides the functionality of a better designed parameter handling than the one provided by the rclcpp::Parameter class.
The ROS2BaseNode includes a ParameterBase and a template Parameter<T> nested class for this purpose. They are extended with the member functions of the ROS2BaseNode
class which handle all the node's parameters and the related issues with the help of a heterogeneous collection.

One can use the class by deriving from it.
Parameters have to be created individually in a shared_ptr form with the correspondent arguments: name, default value,
a ParameterAccessRights structure defining in which states is the setting of the Parameter allowed, the callback to call on setting the Parameter and the node itself.
Lastly they have to be added to the base node with its registerParameter function.
