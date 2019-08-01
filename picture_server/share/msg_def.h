#pragma once
#include <iostream>
#include <QString>
const std::string identifying_code = "{5bda21ee-c048-447d-a5b4-c819563c3000}";
const QString save_dir = "image/";

enum e_msg_type:char {
	upload = 1,
	download = 2,
	upload_return = 3,
	download_return = 4,
    delete_picture = 5,
    delete_picture_return = 6,
    failed = 7,
};


struct Head{
	e_msg_type msg_type;
	char identify_code[38];
	char image_uuid[38];
	int image_len;
	Head() {
		memcpy(this->identify_code, identifying_code.c_str(), 38);
	}
};

#define HEADLEN sizeof(Head)
