// Copyright 2018 Avsyankaa <Avsyankaa@gmail.com>

#include <gtest/gtest.h>

#include <client.hpp>

TEST(talk_to_srv, Test1) {
    auto talk = std::make_shared<talk_to_svr>("Asya");
    std::string name = talk->username();
    EXPECT_EQ("Asya", name);
}

