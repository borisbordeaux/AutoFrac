#include <gtest/gtest.h>

#include "app/bcifs/formalcoef.h"

TEST(FormalCoef, IndexZeroAndOne) {
    BCIFS::FormalCoef zero = BCIFS::FormalCoef::zero();
    BCIFS::FormalCoef one = BCIFS::FormalCoef::one();
    BCIFS::FormalCoef var(4);
    EXPECT_EQ(zero.index(), 0);
    EXPECT_EQ(one.index(), 1);
    EXPECT_EQ(var.index(), 4);
}