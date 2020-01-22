// Copyright 2018 Your Name <your_email>

#include <gtest/gtest.h>

#include <my_shared_ptr.hpp>

TEST(SHARED_PTR, init) {
    SharedPtr<int> ptr;

    EXPECT_FALSE(ptr);
    EXPECT_EQ(ptr.use_count(), 0);

    int *value = new int(5);
    SharedPtr<int> ptr2(value);

    EXPECT_EQ(ptr2.use_count(), 1);
    EXPECT_EQ(*ptr2, 5);
    EXPECT_TRUE(ptr2);

    SharedPtr<int> ptr3(ptr2);

    EXPECT_EQ(ptr2.use_count(), 2);
    EXPECT_EQ(ptr3.use_count(), 2);

    EXPECT_EQ(*ptr3, 5);
    EXPECT_TRUE(ptr3);
}

TEST(SHARED_PTR, assign) {
    int *value = new int(8);
    SharedPtr<int> ptr(value);

    int *anoth_val = new int(2);
    SharedPtr<int> ptr1(anoth_val);

    ptr = ptr1;

    EXPECT_EQ(ptr.use_count(), ptr1.use_count());
    EXPECT_EQ(ptr.use_count(), 2);
    EXPECT_EQ(*ptr1, *ptr);
    EXPECT_TRUE(ptr1);
}

TEST(SHARED_PRT, get) {
    int *value = new int(3);
    SharedPtr<int> ptr(value);

    EXPECT_EQ(ptr.get(), value);

    SharedPtr<int> ptr2;

    EXPECT_EQ(ptr2.get(), nullptr);
}

TEST(SHARED_PTR, reset) {
    int *value = new int(3);
    SharedPtr<int> ptr(value);

    SharedPtr<int> ptr1(ptr);

    EXPECT_EQ(ptr.use_count(), 2);

    ptr.reset();

    EXPECT_FALSE(ptr);
    EXPECT_EQ(ptr.use_count(), 0);
    EXPECT_EQ(ptr1.use_count(), 1);

    int *val = new int(10);
    ptr1.reset(val);

    EXPECT_EQ(ptr1.use_count(), 1);
    EXPECT_EQ(*ptr1, 10);
}

TEST(SHARED_PTR, reset_move) {
    int *value = new int(3);
    int *value2 = new int(5);

    SharedPtr<int> ptr(value);

    ptr.reset(value2);
    EXPECT_EQ(ptr.use_count(), 1);
    EXPECT_EQ(*ptr, 5);
}

TEST(SHARED_PTR, swap) {
    int *value = new int(3);
    SharedPtr<int> pointer1(value);

    int *value2 = new int(10);
    SharedPtr<int> pointer2(value2);
    pointer2.swap(pointer1);

    EXPECT_EQ(pointer2.use_count(), 1);
    EXPECT_EQ(pointer1.use_count(), 1);

    EXPECT_EQ(*pointer1, 10);
    EXPECT_EQ(*pointer2, 3);
}
