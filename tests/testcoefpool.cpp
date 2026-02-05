#include <gtest/gtest.h>
#include "app/bcifs/coefpool.h"

TEST(CoefPool, ConstructorAddsZeroAndOne) {
    BCIFS::CoefPool pool;
    EXPECT_EQ(0.0f, pool.value(0));
    EXPECT_EQ(1.0f, pool.value(1));
    EXPECT_TRUE(pool.isZero(0));
    EXPECT_TRUE(pool.isOne(1));
}

TEST(CoefPool, MakeVarAndConst) {
    BCIFS::CoefPool pool;
    std::size_t indexVar = pool.makeVar(0.3f);
    std::size_t indexConst = pool.makeConst(0.7f);
    EXPECT_EQ(0.3f, pool.value(indexVar));
    EXPECT_EQ(0.7f, pool.value(indexConst));
    EXPECT_TRUE(pool.isVar(indexVar));
    EXPECT_TRUE(pool.isConst(indexConst));
}

TEST(CoefPool, UnifyConstConst) {
    // should throw when value is different
    BCIFS::CoefPool pool;
    std::size_t indexConst1 = pool.makeConst(0.5f);
    std::size_t indexConst2 = pool.makeConst(0.3f);
    std::size_t indexConst3 = pool.makeConst(0.5f);
    EXPECT_NO_THROW(pool.unify(0, 0));
    EXPECT_ANY_THROW(pool.unify(0, 1));
    EXPECT_ANY_THROW(pool.unify(0, indexConst1));
    EXPECT_ANY_THROW(pool.unify(1, 0));
    EXPECT_NO_THROW(pool.unify(1, 1));
    EXPECT_ANY_THROW(pool.unify(1, indexConst1));
    EXPECT_ANY_THROW(pool.unify(indexConst1, 0));
    EXPECT_ANY_THROW(pool.unify(indexConst1, 1));
    EXPECT_NO_THROW(pool.unify(indexConst1, indexConst1));
    EXPECT_ANY_THROW(pool.unify(indexConst1, indexConst2));
    EXPECT_NO_THROW(pool.unify(indexConst1, indexConst3));
    EXPECT_ANY_THROW(pool.unify(indexConst2, indexConst1));
    EXPECT_NO_THROW(pool.unify(indexConst2, indexConst2));
    EXPECT_NO_THROW(pool.unify(indexConst3, indexConst1));
}

TEST(CoefPool, UnifyVarConst) {
    // the var should take the value of the const
    // and the const should not change
    BCIFS::CoefPool pool;
    std::size_t indexConst = pool.makeConst(0.5f);
    std::size_t indexVar = pool.makeVar(0.3f);
    pool.unify(indexVar, indexConst);
    EXPECT_EQ(pool.value(indexVar), 0.5f);
    EXPECT_EQ(pool.isConst(indexVar), true);
    EXPECT_EQ(pool.value(indexConst), 0.5f);
    EXPECT_EQ(pool.isConst(indexConst), true);
}

TEST(CoefPool, UnifyConstVar) {
    // the var should take the value of the const
    // and the const should not change
    BCIFS::CoefPool pool;
    std::size_t indexConst = pool.makeConst(0.5f);
    std::size_t indexVar = pool.makeVar(0.3f);
    pool.unify(indexConst, indexVar);
    EXPECT_EQ(pool.value(indexVar), 0.5f);
    EXPECT_EQ(pool.isConst(indexVar), true);
    EXPECT_EQ(pool.value(indexConst), 0.5f);
    EXPECT_EQ(pool.isConst(indexConst), true);
}

TEST(CoefPool, UnifyVarVar) {
    // the second var should take the value of the first one
    BCIFS::CoefPool pool;
    std::size_t indexVar1 = pool.makeVar(0.5f);
    std::size_t indexVar2 = pool.makeVar(0.3f);
    pool.unify(indexVar1, indexVar2);
    EXPECT_EQ(pool.value(indexVar1), 0.5f);
    EXPECT_EQ(pool.isVar(indexVar1), true);
    EXPECT_EQ(pool.value(indexVar2), 0.5f);
    EXPECT_EQ(pool.isVar(indexVar2), true);
}

TEST(CoefPool, RootAfterUnificationVarVar) {
    // the first var should be the root of the second one
    BCIFS::CoefPool pool;
    std::size_t indexVar1 = pool.makeVar(0.5f);
    std::size_t indexVar2 = pool.makeVar(0.3f);
    pool.unify(indexVar1, indexVar2);
    EXPECT_EQ(pool.root(indexVar1), indexVar1);
    EXPECT_EQ(pool.root(indexVar2), indexVar1);
}

TEST(CoefPool, RootAfterUnificationConstVar) {
    // the const should be the root of the var
    BCIFS::CoefPool pool;
    std::size_t indexConst = pool.makeConst(0.5f);
    std::size_t indexVar = pool.makeVar(0.3f);
    pool.unify(indexConst, indexVar);
    EXPECT_EQ(pool.root(indexVar), indexConst);
    EXPECT_EQ(pool.root(indexConst), indexConst);
}

TEST(CoefPool, Multiply) {
    BCIFS::CoefPool pool;
    std::size_t indexConst1 = pool.makeConst(0.5f);
    std::size_t indexConst2 = pool.makeConst(0.3f);
    std::size_t indexVar1 = pool.makeVar(0.2f);
    std::size_t indexVar2 = pool.makeVar(0.4f);
    EXPECT_EQ(pool.mul(0, indexConst1), 0);
    EXPECT_EQ(pool.mul(0, indexVar1), 0);
    EXPECT_EQ(pool.mul(0, 0), 0);
    EXPECT_EQ(pool.mul(0, 1), 0);
    EXPECT_EQ(pool.mul(indexConst1, 0), 0);
    EXPECT_EQ(pool.mul(indexVar1, 0), 0);
    EXPECT_EQ(pool.mul(1, 0), 0);
    EXPECT_EQ(pool.mul(1, 1), 1);
    EXPECT_EQ(pool.mul(1, indexConst1), indexConst1);
    EXPECT_EQ(pool.mul(1, indexVar1), indexVar1);
    EXPECT_EQ(pool.mul(indexConst1, 1), indexConst1);
    EXPECT_EQ(pool.mul(indexVar1, 1), indexVar1);
    EXPECT_ANY_THROW(pool.mul(indexConst1, indexConst2));
    EXPECT_ANY_THROW(pool.mul(indexVar1, indexVar2));
}

TEST(CoefPool, Addition) {
    BCIFS::CoefPool pool;
    std::size_t indexConst1 = pool.makeConst(0.5f);
    std::size_t indexConst2 = pool.makeConst(0.3f);
    std::size_t indexVar1 = pool.makeVar(0.2f);
    std::size_t indexVar2 = pool.makeVar(0.4f);
    EXPECT_EQ(pool.add(0, indexConst1), indexConst1);
    EXPECT_EQ(pool.add(0, indexVar1), indexVar1);
    EXPECT_EQ(pool.add(0, 0), 0);
    EXPECT_EQ(pool.add(0, 1), 1);
    EXPECT_EQ(pool.add(indexConst1, 0), indexConst1);
    EXPECT_EQ(pool.add(indexVar1, 0), indexVar1);
    EXPECT_EQ(pool.add(1, 0), 1);
    EXPECT_ANY_THROW(pool.add(1, 1));
    EXPECT_ANY_THROW(pool.add(1, indexConst1));
    EXPECT_ANY_THROW(pool.add(1, indexVar1));
    EXPECT_ANY_THROW(pool.add(indexConst1, 1));
    EXPECT_ANY_THROW(pool.add(indexVar1, 1));
    EXPECT_ANY_THROW(pool.add(indexConst1, indexConst2));
    EXPECT_ANY_THROW(pool.add(indexVar1, indexVar2));
}