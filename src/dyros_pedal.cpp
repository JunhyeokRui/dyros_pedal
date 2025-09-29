#include "../include/dyros_pedal/dyros_pedal.h"

DyrosPedal::DyrosPedal()
{
    std::string joy_topic_name; 
    if (!nh_.getParam("joy_topic", joy_topic_name)) {
        joy_topic_name = "joy";
    }
    std::cout<<"topic name: "<< joy_topic_name <<  std::endl;
    walking_cmd_pub_ = nh_.advertise<tocabi_msgs::WalkingCommand>("/joy_wh", 100);
    pedal_sub_ = nh_.subscribe<sensor_msgs::Joy>(joy_topic_name, 10, &DyrosPedal::pedalCallback, this);
    pedal_sub2_ = nh_.subscribe<sensor_msgs::Joy>("joy_deck", 10, &DyrosPedal::deckCallback, this);
}

void DyrosPedal::deckCallback(const sensor_msgs::Joy::ConstPtr &joy)
{
    walk_cmd_msg_.step_length_x = joy->axes[1];



    walk_cmd_msg_.z = joy->axes[1];


    walk_cmd_msg_.theta = joy->axes[3];

    if (!use_deck_)
    {
        if (joy->buttons[5])
        {
            use_deck_ = true;
        }
    }

    if (use_deck_)
    {
        if (joy->buttons[5])
        {
            walking_cmd_pub_.publish(walk_cmd_msg_);
        }
        else
        {
            use_deck_ = false;
        }
    }

    if(!joy->buttons[5])
    {
        use_deck_ = false;
    }
}

void DyrosPedal::pedalCallback(const sensor_msgs::Joy::ConstPtr &pedal)
{
    bool playstation_joystick_mode = false;
    bool pedal_mode = true;

    //    if (pedal->axes[1] > 0.5)
    //    {
    //        walk_cmd_ = true;
    //        walk_cmd_msg_.x = 1.0;
    //    }
    //    else
    //    {
    //        walk_cmd_ = false;
    //        walk_cmd_msg_.x = 0.0;
    //    }
    // if(pedal->buttons[8] == 1.0){
    //     walk_cmd_ = true;
    // }
    // if(pedal->buttons[6] == 1.0){
    //     walk_cmd_ =false;
    // }

    // walk_cmd_msg_.x = pedal->axes[1];
    // walk_cmd_msg_.y = pedal->axes[3];rostopic publish 가 되는 topic 이름을 바꿔야 한다. → 이는 tocabi_rm 컴퓨터에 pedal_ws 폴더 내에 joy_wh.launch에 구현되어 있다. 
    // walk_cmd_msg_.z = pedal->axes[2]; //default is 1.0, when the button pushed, -1.0
    // std::cout << pedal->axes[3] << std::endl;
    // walk_cmd_msg_.walk_mode = walk_cmd_;
    // walk_cmd_msg_.height = 0.0;
    // walk_cmd_msg_.height = pedal->axes[0];
    // walk_cmd_msg_.theta = pedal->axes[4];
    // walk_cmd_msg_.step_length_x = pedal->buttons[1];
    // walk_cmd_msg_.step_length_y = pedal->buttons[2];
    // std::cout << "B" <<walk_cmd_msg_.theta << "X" <<pedal->axes[4] << std::endl;

    // pedal mode

    if (pedal_mode)
    {
        walk_cmd_msg_.step_length_x = pedal->axes[0];
        walk_cmd_msg_.z = pedal->axes[1];
        walk_cmd_msg_.theta = pedal->axes[2];

        // walk_cmd_msg_.step_length_x = pedal->axes[1] * 2 - 1;
        // walk_cmd_msg_.z = pedal->axes[1] * 2 - 1;
        // walk_cmd_msg_.theta = - pedal->axes[3];
    }

    // play station controller mode'
    if (playstation_joystick_mode)
    {
        if (pedal->axes[1] >= 0)
        {
            walk_cmd_msg_.step_length_x = 2 * pedal->axes[1] - 1;
            walk_cmd_msg_.z = -1.0;
        }
        else
        {
            walk_cmd_msg_.step_length_x = -1.0;
            walk_cmd_msg_.z = 2 * (-pedal->axes[1]) - 1;
        }

        walk_cmd_msg_.theta = -pedal->axes[3];
    }

    // if (walk_cmd_ ==true)
    // if (!use_deck_)
    // {
        // std::cout << "Pedal Started" << std::endl;
        walking_cmd_pub_.publish(walk_cmd_msg_);
    // }
    // std::cout << "step_length : " << walk_cmd_msg_.step_length_x << std::endl;
    // std::cout << "trigger(z) : " << walk_cmd_msg_.z << std::endl;
    // std::cout << "theta : " << walk_cmd_msg_.theta << std::endl << std::endl;
}

int main(int argc, char **argv)
{
    std::cout << "Pedal Started" << std::endl;
    ros::init(argc, argv, "dyros_pedal");
    DyrosPedal dyrospedal;
    std::cout << "Pedal Controller Started" << std::endl;
    while (ros::ok())
    {
        ros::spinOnce();
    }
}
