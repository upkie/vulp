// SPDX-License-Identifier: Apache-2.0
// Copyright 2022 Stéphane Caron

#pragma once

#include <palimpsest/Dictionary.h>
#include <spdlog/spdlog.h>

#include <limits>
#include <map>
#include <string>
#include <vector>

#include "RobotSimulator/b3RobotSimulatorClientAPI.h"
#include "vulp/actuation/BulletImuData.h"
#include "vulp/actuation/BulletJointProperties.h"
#include "vulp/actuation/Interface.h"
#include "vulp/actuation/moteus/Output.h"
#include "vulp/actuation/moteus/ServoReply.h"

namespace vulp::actuation {

//! Actuation interface for the Bullet simulator.
class BulletInterface : public Interface {
 public:
  //! Interface parameters.
  struct Parameters {
    //! Keep default constructor.
    Parameters() = default;

    /*! Initialize from global configuration.
     *
     * \param[in] config Global configuration dictionary.
     */
    explicit Parameters(const Dictionary& config) { configure(config); }

    /*! Configure from dictionary.
     *
     * \param[in] config Global configuration dictionary.
     */
    void configure(const Dictionary& config) {
      if (!config.has("bullet")) {
        spdlog::debug("No \"bullet\" runtime configuration");
        return;
      }
      spdlog::info("Applying \"bullet\" runtime configuration...");

      const auto& bullet = config("bullet");
      follower_camera = bullet.get<bool>("follower_camera", follower_camera);
      gui = bullet.get<bool>("gui", gui);

      groundtruth_contacts.clear();
      if (bullet.has("groundtruth")) {
        const auto& groundtruth = bullet("groundtruth");
        if (groundtruth.has("contacts")) {
          for (const auto& body : groundtruth("contacts").keys()) {
            spdlog::info("Adding body {} to contacts", body);
            groundtruth_contacts.push_back(body);
          }
        }
      }

      joint_friction.clear();
      if (bullet.has("joint_properties")) {
        for (const auto& joint : bullet("joint_properties").keys()) {
          const auto& props = bullet("joint_properties")(joint);
          if (props.has("friction")) {
            joint_friction.try_emplace(joint, props.get<double>("friction"));
          }
        }
      }

      if (bullet.has("reset")) {
        const auto& reset = bullet("reset");
        position_base_in_world = reset.get<Eigen::Vector3d>(
            "position_base_in_world", Eigen::Vector3d::Zero());
        orientation_base_in_world = reset.get<Eigen::Quaterniond>(
            "orientation_base_in_world", Eigen::Quaterniond::Identity());
        linear_velocity_base_to_world_in_world = reset.get<Eigen::Vector3d>(
            "linear_velocity_base_to_world_in_world", Eigen::Vector3d::Zero());
        angular_velocity_base_in_base = reset.get<Eigen::Vector3d>(
            "angular_velocity_base_in_base", Eigen::Vector3d::Zero());
      }

      if (bullet.has("torque_control")) {
        torque_control_kd = bullet("torque_control")("kd");
        torque_control_kp = bullet("torque_control")("kp");
      }
    }

    /*! Value of argv[0] used to locate runfiles (e.g. plane.urdf) in Bazel.
     *
     * This value helps find runfiles because Bazel does not seem to set the
     * RUNFILES_MANIFEST_FILE environment variable from cc_binary rules,
     * although in a similar context it does set it from py_binary rules that
     * depend on "@rules_python//python/runfiles". When RUNFILES_MANIFEST_FILE
     * is unset, knowing argv[0] triggers an alternative way to find runfiles.
     *
     * The following issues are related:
     *
     * https://github.com/bazelbuild/bazel/issues/4586
     * https://github.com/bazelbuild/bazel/issues/7994
     */
    std::string argv0 = "";

    //! Contacts to report along with observations
    std::vector<std::string> groundtruth_contacts;

    //! Simulation timestep in [s]
    double dt = std::numeric_limits<double>::quiet_NaN();

    //! Translate the camera to follow the robot
    bool follower_camera = false;

    //! If true, set gravity to -9.81 m/s².
    bool gravity = true;

    //! If true, load a floor plane.
    bool floor = true;

    //! If true, fire up the graphical user interface.
    bool gui = false;

    /*! Path to the URDF model of the robot.
     *
     * A path from the root of the Bazel workspace works. For instance, use
     * "models/upkie_description/urdf/upkie.urdf" to load the URDF from Bazel
     * data such as ``data = ["//models/upkie_description"]``.
     *
     * For external targets, add the "external/" prefix. For instance, use
     * "external/upkie_description/urdf/upkie.urdf" to load the URDF from Bazel
     * data loaded from a dependency: ``data = ["@upkie_description"]``.
     */
    std::string robot_urdf_path;

    //! Paths to environment URDFs to load.
    std::vector<std::string> env_urdf_paths;

    //! Gain for joint velocity control feedback
    double torque_control_kd = 1.0;

    //! Gain for joint position control feedback
    double torque_control_kp = 20.0;

    //! Position of the base in the world frame upon reset
    Eigen::Vector3d position_base_in_world = Eigen::Vector3d::Zero();

    //! Orientation of the base in the world frame upon reset
    Eigen::Quaterniond orientation_base_in_world =
        Eigen::Quaterniond::Identity();

    //! Linear velocity of the base in the world frame upon reset
    Eigen::Vector3d linear_velocity_base_to_world_in_world =
        Eigen::Vector3d::Zero();

    //! Body angular velocity of the base upon reset
    Eigen::Vector3d angular_velocity_base_in_base = Eigen::Vector3d::Zero();

    //! Joint friction parameters
    std::map<std::string, double> joint_friction;
  };

  /*! Initialize interface.
   *
   * \param[in] layout Servo layout.
   * \param[in] params Interface parameters.
   *
   * \throw std::runtime_error If the simulator did not start properly.
   */
  BulletInterface(const ServoLayout& layout, const Parameters& params);

  //! Disconnect interface.
  ~BulletInterface();

  /*! Reset interface.
   *
   * \param[in] config Additional configuration dictionary.
   */
  void reset(const Dictionary& config) override;

  /*! Spin a new communication cycle.
   *
   * \param data Buffer to read commands from and write replies to.
   * \param callback Function to call when the cycle is over.
   *
   * The callback will be invoked from an arbitrary thread when the
   * communication cycle has completed. All memory pointed to by @p data must
   * remain valid until the callback is invoked.
   */
  void cycle(const moteus::Data& data,
             std::function<void(const moteus::Output&)> callback) final;

  /*! Write actuation-interface observations to dictionary.
   *
   * \param[out] observation Dictionary to write ot.
   */
  void observe(Dictionary& observation) const override;

  //! Get the groundtruth floating base transform
  Eigen::Matrix4d transform_base_to_world() const noexcept;

  /*! Get the groundtruth floating base linear velocity
   *
   * \note This function is only used for testing and does not need to be
   * optimized.
   */
  Eigen::Vector3d linear_velocity_base_to_world_in_world() const noexcept;

  /*! Get the groundtruth floating base angular velocity
   *
   * \note This function is only used for testing and does not need to be
   * optimized.
   */
  Eigen::Vector3d angular_velocity_base_in_base() const noexcept;

  //! Reset joint angles to zero.
  void reset_joint_angles();

  //! Reset joint properties to defaults.
  void reset_joint_properties();

  /*! Reset the pose and velocity of the floating base in the world frame.
   *
   * \param[in] position_base_in_world Position of the base in the world frame.
   * \param[in] orientation_base_in_world Orientation of the base in the world
   *     frame.
   * \param[in] linear_velocity_base_to_world_in_world Linear velocity of the
   *     base in the world frame.
   * \param[in] angular_velocity_base_in_base Body angular velocity of the base
   *     (in the base frame).
   */
  void reset_base_state(
      const Eigen::Vector3d& position_base_in_world,
      const Eigen::Quaterniond& orientation_base_in_world,
      const Eigen::Vector3d& linear_velocity_base_to_world_in_world,
      const Eigen::Vector3d& angular_velocity_base_in_base);

  //! Joint properties (accessor used for testing)
  const std::map<std::string, BulletJointProperties>& joint_properties() {
    return joint_properties_;
  }

  //! Internal map of servo replies (accessor used for testing)
  const std::map<std::string, moteus::ServoReply>& servo_reply() {
    return servo_reply_;
  }

  /*! Reproduce the moteus position controller in Bullet.
   *
   * \param[in] joint_name Name of the joint.
   * \param[in] feedforward_torque Feedforward torque command in [N] * [m].
   * \param[in] target_position Target angular position in [rad].
   * \param[in] target_velocity Target angular velocity in [rad] / [s].
   * \param[in] kp_scale Multiplicative factor applied to the proportional gain
   *     in torque control.
   * \param[in] kd_scale Multiplicative factor applied to the derivative gain
   *     in torque control.
   * \param[in] maximum_torque Maximum torque in [N] * [m] from the command.
   */
  double compute_joint_torque(const std::string& joint_name,
                              const double feedforward_torque,
                              const double target_position,
                              const double target_velocity,
                              const double kp_scale, const double kd_scale,
                              const double maximum_torque);

 private:
  /*! Get index of a given robot link in Bullet
   *
   * \param[in] link_name Name of the searched link.
   *
   * \return Link index if found, -1 otherwise.
   *
   * This is a memoization wrapper around \ref find_link_index, thus it has
   * amortized O(log n) time complexity (that of std::map, where n is the
   * number of links).
   */
  int get_link_index(const std::string& link_name);

  //! Compute IMU readings from the IMU link state.
  void read_imu_data();

  //! Read joint sensors from the simulator
  void read_joint_sensors();

  //! Report collisions between pairs specified in input parameters
  void report_contacts();

  /*! Send commands to simulated joints
   *
   * \param data Buffer to read commands from.
   */
  void send_commands(const moteus::Data& data);

  //! Convenience function to follow the base translation
  void translate_camera_to_robot();

 private:
  //! Interface parameters
  Parameters params_;

  //! Map from actuated joint name to joint index in Bullet
  std::map<std::string, int> joint_index_map_;

  //! Simulated servo replies, laid out as a joint_name -> servo_reply map
  std::map<std::string, moteus::ServoReply> servo_reply_;

  //! Bullet client
  b3RobotSimulatorClientAPI bullet_;

  //! Identifier of the robot model in the simulation
  int robot_;

  //! Maximum joint torques read from the URDF model
  std::map<std::string, BulletJointProperties> joint_properties_;

  //! Link index of the IMU in Bullet
  int imu_link_index_;

  //! IMU data
  BulletImuData imu_data_;

  //! Spatial linear velocity of the IMU link, used to compute its acceleration
  Eigen::Vector3d linear_velocity_imu_in_world_;

  //! Map from link name to link index in Bullet
  std::map<std::string, int> link_index_;
};

}  // namespace vulp::actuation
