-- states
state("V", 1)
state("B", 1)
state("C", 0)
state("F", 0)
state("Vol", 0)
state("WB", 0)
state("WC", 0)
state("L", 0)

-- permutations
permutation("p0", "C", "C")
permutation("p0", "B", "B")
permutation("p0", "L", "L")
permutation("p0", "WC", "WC")
permutation("p0", "WB", "WB")

-- boundary of states
boundary("b0", "B", "V")
boundary("b1", "B", "V")
boundary("b0", "L", "V")
boundary("b1", "L", "V")
boundary("b0", "C", "V")
boundary("b1", "C", "V")
boundary("b0", "F", "C")
boundary("b1", "F", "B")
boundary("b2", "F", "C")
boundary("b3", "F", "B")
boundary("b4", "F", "C")
boundary("b5", "F", "B")
boundary("b0", "WB", "B")
boundary("b1", "WB", "L")
boundary("b2", "WB", "B")
boundary("b3", "WB", "L")
boundary("b0", "WC", "C")
boundary("b1", "WC", "L")
boundary("b2", "WC", "C")
boundary("b3", "WC", "L")
boundary("b0", "Vol", "F")
boundary("b1", "Vol", "F")
boundary("b2", "Vol", "WC")
boundary("b3", "Vol", "WB")
boundary("b4", "Vol", "WC")
boundary("b5", "Vol", "WB")
boundary("b6", "Vol", "WC")
boundary("b7", "Vol", "WB")

-- grid of states
grid("F", {
    { { "b0", "b0" }, { "b0", "b1" } },
    { { "b1", "b0" }, { "b1", "intern_0" }, { "b1", "b1" } },
    { { "b2", "b0" }, { "b2", "b1" } },
    { { "b3", "b0" }, { "b3", "intern_0" }, { "b3", "b1" } },
    { { "b4", "b0" }, { "b4", "b1" } },
    { { "b5", "b0" }, { "b5", "intern_0" }, { "b5", "b1" } }
})

grid("WB", {
    { { "b0", "b0" }, { "b0", "intern_0" }, { "b0", "b1" } },
    { { "b1", "b0" }, { "b1", "b1" } },
    { { "b2", "b0" }, { "b2", "intern_0" }, { "b2", "b1" } },
    { { "b3", "b0" }, { "b3", "b1" } }
})

grid("WC", {
    { { "b0", "b0" }, { "b0", "b1" } },
    { { "b1", "b0" }, { "b1", "b1" } },
    { { "b2", "b0" }, { "b2", "b1" } },
    { { "b3", "b0" }, { "b3", "b1" } }
})

grid("Vol", {
    { { "b0", "b0", "b0" }, { "b0", "b0", "b1" } },
    { { "b0", "b1", "b0" }, { "b0", "b1", "intern_0" }, { "b0", "b1", "b1" } },
    { { "b0", "b2", "b0" }, { "b0", "b2", "b1" } },
    { { "b0", "b3", "b0" }, { "b0", "b3", "intern_0" }, { "b0", "b3", "b1" } },
    { { "b0", "b4", "b0" }, { "b0", "b4", "b1" } },
    { { "b0", "b5", "b0" }, { "b0", "b5", "intern_0" }, { "b0", "b5", "b1" } },

    { { "b1", "b0", "b0" }, { "b1", "b0", "b1" } },
    { { "b1", "b1", "b0" }, { "b1", "b1", "intern_0" }, { "b1", "b1", "b1" } },
    { { "b1", "b2", "b0" }, { "b1", "b2", "b1" } },
    { { "b1", "b3", "b0" }, { "b1", "b3", "intern_0" }, { "b1", "b3", "b1" } },
    { { "b1", "b4", "b0" }, { "b1", "b4", "b1" } },
    { { "b1", "b5", "b0" }, { "b1", "b5", "intern_0" }, { "b1", "b5", "b1" } },

    { { "b2", "b0", "b0" }, { "b2", "b0", "b1" } },
    { { "b2", "b1", "b0" }, { "b2", "b1", "b1" } },
    { { "b2", "b2", "b0" }, { "b2", "b2", "b1" } },
    { { "b2", "b3", "b0" }, { "b2", "b3", "b1" } },

    { { "b3", "b0", "b0" }, { "b3", "b0", "intern_0" }, { "b3", "b0", "b1" } },
    { { "b3", "b1", "b0" }, { "b3", "b1", "b1" } },
    { { "b3", "b2", "b0" }, { "b3", "b2", "intern_0" }, { "b3", "b2", "b1" } },
    { { "b3", "b3", "b0" }, { "b3", "b3", "b1" } },

    { { "b4", "b0", "b0" }, { "b4", "b0", "b1" } },
    { { "b4", "b1", "b0" }, { "b4", "b1", "b1" } },
    { { "b4", "b2", "b0" }, { "b4", "b2", "b1" } },
    { { "b4", "b3", "b0" }, { "b4", "b3", "b1" } },

    { { "b5", "b0", "b0" }, { "b5", "b0", "intern_0" }, { "b5", "b0", "b1" } },
    { { "b5", "b1", "b0" }, { "b5", "b1", "b1" } },
    { { "b5", "b2", "b0" }, { "b5", "b2", "intern_0" }, { "b5", "b2", "b1" } },
    { { "b5", "b3", "b0" }, { "b5", "b3", "b1" } },

    { { "b6", "b0", "b0" }, { "b6", "b0", "b1" } },
    { { "b6", "b1", "b0" }, { "b6", "b1", "b1" } },
    { { "b6", "b2", "b0" }, { "b6", "b2", "b1" } },
    { { "b6", "b3", "b0" }, { "b6", "b3", "b1" } },

    { { "b7", "b0", "b0" }, { "b7", "b0", "intern_0" }, { "b7", "b0", "b1" } },
    { { "b7", "b1", "b0" }, { "b7", "b1", "b1" } },
    { { "b7", "b2", "b0" }, { "b7", "b2", "intern_0" }, { "b7", "b2", "b1" } },
    { { "b7", "b3", "b0" }, { "b7", "b3", "b1" } }
})

-- space of states
space("B", { "b0", "intern_0", "b1" })
space("C", { "b0", "b1" })
space("L", { "b0", "b1" })
space("F", { "b0", "b1", "b2", "b3", "b4", "b5" })
space("Vol", { "b0", "b1", "b2", "b3", "b4", "b5", "b6", "b7" })

-- subdivision of states
subdivision("s0", "V", "V")
subdivision("s0", "B", "B")
subdivision("s1", "B", "B")
subdivision("s0", "L", "L")
subdivision("s1", "L", "L")
subdivision("s0", "C", "C")
subdivision("s1", "C", "C")
subdivision("s0", "F", "F")
subdivision("s1", "F", "F")
subdivision("s2", "F", "F")
subdivision("s3", "F", "F")
subdivision("s4", "F", "F")
subdivision("s5", "F", "F")
subdivision("s0", "WB", "WB")
subdivision("s1", "WB", "WB")
subdivision("s2", "WB", "WB")
subdivision("s3", "WB", "WB")
subdivision("s0", "WC", "WC")
subdivision("s1", "WC", "WC")
subdivision("s2", "WC", "WC")
subdivision("s3", "WC", "WC")
subdivision("s0", "Vol", "Vol", { 252/255, 127/255, 0 })
subdivision("s1", "Vol", "Vol", { 0, 71/255, 232/255 })
subdivision("s2", "Vol", "Vol", { 205/255, 207/255, 0 })
subdivision("s3", "Vol", "Vol", { 206/255, 0, 0 })
subdivision("s4", "Vol", "Vol", { 0, 183/255, 0 })
subdivision("s5", "Vol", "Vol", { 129/255, 50/255, 1 })
subdivision("s6", "Vol", "Vol", { 252/255, 127/255, 0 })
subdivision("s7", "Vol", "Vol", { 0, 71/255, 232/255 })
subdivision("s8", "Vol", "Vol", { 205/255, 207/255, 0 })
subdivision("s9", "Vol", "Vol", { 206/255, 0, 0 })
subdivision("s10", "Vol", "Vol", { 0, 183/255, 0 })
subdivision("s11", "Vol", "Vol", { 129/255, 50/255, 1 })
subdivision("s0", "init", "Vol")

-- primitive of states
primitive("Vol", {
    {
        { "b0", "b0", "b0" },
        { "b0", "b1", "b0" },
        { "b0", "b1", "intern_0" },
        { "b0", "b2", "b0" },
        { "b0", "b3", "b0" },
        { "b0", "b3", "intern_0" },
        { "b0", "b4", "b0" },
        { "b0", "b5", "b0" },
        { "b0", "b5", "intern_0" }
    },
    {
        { "b1", "b5", "intern_0" },
        { "b1", "b5", "b0" },
        { "b1", "b4", "b0" },
        { "b1", "b3", "intern_0" },
        { "b1", "b3", "b0" },
        { "b1", "b2", "b0" },
        { "b1", "b1", "intern_0" },
        { "b1", "b1", "b0" },
        { "b1", "b0", "b0" },
    },
    {
        { "b2", "b0", "b0" },
        { "b2", "b1", "b0" },
        { "b2", "b2", "b0" },
        { "b2", "b3", "b0" },
    },
    {
        { "b3", "b0", "b0" },
        { "b3", "b0", "intern_0" },
        { "b3", "b2", "intern_0" },
        { "b3", "b3", "b0" },
    },
    {
        { "b3", "b0", "intern_0" },
        { "b3", "b1", "b0" },
        { "b3", "b2", "b0" },
        { "b3", "b2", "intern_0" },
    },
    {
        { "b4", "b0", "b0" },
        { "b4", "b1", "b0" },
        { "b4", "b2", "b0" },
        { "b4", "b3", "b0" },
    },
    {
        { "b5", "b0", "b0" },
        { "b5", "b0", "intern_0" },
        { "b5", "b2", "intern_0" },
        { "b5", "b3", "b0" },
    },
    {
        { "b5", "b0", "intern_0" },
        { "b5", "b1", "b0" },
        { "b5", "b2", "b0" },
        { "b5", "b2", "intern_0" },
    },
    {
        { "b6", "b0", "b0" },
        { "b6", "b1", "b0" },
        { "b6", "b2", "b0" },
        { "b6", "b3", "b0" },
    },
    {
        { "b7", "b0", "b0" },
        { "b7", "b0", "intern_0" },
        { "b7", "b2", "intern_0" },
        { "b7", "b3", "b0" },
    },
    {
        { "b7", "b0", "intern_0" },
        { "b7", "b1", "b0" },
        { "b7", "b2", "b0" },
        { "b7", "b2", "intern_0" },
    }
})

-- permutation constraints
-- to define permutation operators
constraint("C", { "p0", "b0" }, { "b1" })
constraint("C", { "p0", "b1" }, { "b0" })
constraint("L", { "p0", "b0" }, { "b1" })
constraint("L", { "p0", "b1" }, { "b0" })
constraint("B", { "p0", "b0" }, { "b1" })
constraint("B", { "p0", "intern_0" }, { "intern_0" })
constraint("B", { "p0", "b1" }, { "b0" })
constraint("WC", { "p0", "b0" }, { "b0", "p0" })
constraint("WC", { "p0", "b1" }, { "b3", "p0" })
constraint("WC", { "p0", "b2" }, { "b2", "p0" })
constraint("WC", { "p0", "b3" }, { "b1", "p0" })
constraint("WB", { "p0", "b0" }, { "b0", "p0" })
constraint("WB", { "p0", "b1" }, { "b3", "p0" })
constraint("WB", { "p0", "b2" }, { "b2", "p0" })
constraint("WB", { "p0", "b3" }, { "b1", "p0" })
-- to constrain subdivision operators using permutation operators
constraint("C", { "p0", "s0" }, { "s1", "p0" })
constraint("C", { "p0", "s1" }, { "s0", "p0" })
constraint("B", { "p0", "s0" }, { "s1", "p0" })
constraint("B", { "p0", "s1" }, { "s0", "p0" })
constraint("L", { "p0", "s0" }, { "s1", "p0" })
constraint("L", { "p0", "s1" }, { "s0", "p0" })
constraint("WC", { "p0", "s0" }, { "s1", "p0" })
constraint("WC", { "p0", "s1" }, { "s0", "p0" })
constraint("WC", { "p0", "s2" }, { "s3", "p0" })
constraint("WC", { "p0", "s3" }, { "s2", "p0" })
constraint("WB", { "p0", "s0" }, { "s1", "p0" })
constraint("WB", { "p0", "s1" }, { "s0", "p0" })
constraint("WB", { "p0", "s2" }, { "s3", "p0" })
constraint("WB", { "p0", "s3" }, { "s2", "p0" })

-- incidence constraints
-- on edge
constraint("C", { "b0", "s0" }, { "s0", "b0" })
constraint("C", { "b1", "s0" }, { "s1", "b1" })
constraint("B", { "b0", "s0" }, { "s0", "b0" })
constraint("B", { "b1", "s0" }, { "s1", "b1" })
-- on face
constraint("F", { "b0", "s0" }, { "s5", "b0" })
constraint("F", { "b0", "s1" }, { "s0", "b0" })
constraint("F", { "b1", "s0" }, { "s0", "b1" })
constraint("F", { "b1", "s1" }, { "s1", "b1" })
constraint("F", { "b2", "s0" }, { "s1", "b2" })
constraint("F", { "b2", "s1" }, { "s2", "b2" })
constraint("F", { "b3", "s0" }, { "s2", "b3" })
constraint("F", { "b3", "s1" }, { "s3", "b3" })
constraint("F", { "b4", "s0" }, { "s3", "b4" })
constraint("F", { "b4", "s1" }, { "s4", "b4" })
constraint("F", { "b5", "s0" }, { "s4", "b5" })
constraint("F", { "b5", "s1" }, { "s5", "b5" })
-- on walls
constraint("WB", { "b0", "s0" }, { "s0", "b0" })
constraint("WB", { "b0", "s1" }, { "s1", "b0" })
constraint("WB", { "b1", "s0" }, { "s1", "b1" })
constraint("WB", { "b1", "s1" }, { "s2", "b1" })
constraint("WB", { "b2", "s0" }, { "s2", "b2" })
constraint("WB", { "b2", "s1" }, { "s3", "b2" })
constraint("WB", { "b3", "s0" }, { "s3", "b3" })
constraint("WB", { "b3", "s1" }, { "s0", "b3" })

constraint("WC", { "b0", "s0" }, { "s0", "b0" })
constraint("WC", { "b0", "s1" }, { "s1", "b0" })
constraint("WC", { "b1", "s0" }, { "s1", "b1" })
constraint("WC", { "b1", "s1" }, { "s2", "b1" })
constraint("WC", { "b2", "s0" }, { "s2", "b2" })
constraint("WC", { "b2", "s1" }, { "s3", "b2" })
constraint("WC", { "b3", "s0" }, { "s3", "b3" })
constraint("WC", { "b3", "s1" }, { "s0", "b3" })
-- on volume
constraint("Vol", { "b0", "s0" }, { "s0", "b0" })
constraint("Vol", { "b0", "s1" }, { "s1", "b0" })
constraint("Vol", { "b0", "s2" }, { "s2", "b0" })
constraint("Vol", { "b0", "s3" }, { "s3", "b0" })
constraint("Vol", { "b0", "s4" }, { "s4", "b0" })
constraint("Vol", { "b0", "s5" }, { "s5", "b0" })

constraint("Vol", { "b1", "s0" }, { "s6", "b1" })
constraint("Vol", { "b1", "s1" }, { "s7", "b1" })
constraint("Vol", { "b1", "s2" }, { "s8", "b1" })
constraint("Vol", { "b1", "s3" }, { "s9", "b1" })
constraint("Vol", { "b1", "s4" }, { "s10", "b1" })
constraint("Vol", { "b1", "s5" }, { "s11", "b1" })

constraint("Vol", { "b2", "s1" }, { "s5", "b2" })
constraint("Vol", { "b2", "s0" }, { "s0", "b2" })
constraint("Vol", { "b3", "s1" }, { "s0", "b3" })
constraint("Vol", { "b3", "s0" }, { "s1", "b3" })
constraint("Vol", { "b4", "s1" }, { "s1", "b4" })
constraint("Vol", { "b4", "s0" }, { "s2", "b4" })
constraint("Vol", { "b5", "s1" }, { "s2", "b5" })
constraint("Vol", { "b5", "s0" }, { "s3", "b5" })
constraint("Vol", { "b6", "s1" }, { "s3", "b6" })
constraint("Vol", { "b6", "s0" }, { "s4", "b6" })
constraint("Vol", { "b7", "s1" }, { "s4", "b7" })
constraint("Vol", { "b7", "s0" }, { "s5", "b7" })

constraint("Vol", { "b2", "s2" }, { "s11", "b2" })
constraint("Vol", { "b2", "s3" }, { "s6", "b2" })
constraint("Vol", { "b3", "s2" }, { "s6", "b3" })
constraint("Vol", { "b3", "s3" }, { "s7", "b3" })
constraint("Vol", { "b4", "s2" }, { "s7", "b4" })
constraint("Vol", { "b4", "s3" }, { "s8", "b4" })
constraint("Vol", { "b5", "s2" }, { "s8", "b5" })
constraint("Vol", { "b5", "s3" }, { "s9", "b5" })
constraint("Vol", { "b6", "s2" }, { "s9", "b6" })
constraint("Vol", { "b6", "s3" }, { "s10", "b6" })
constraint("Vol", { "b7", "s2" }, { "s10", "b7" })
constraint("Vol", { "b7", "s3" }, { "s11", "b7" })

-- adjacency constraints
-- on edge
constraint("B", { "s0", "b1" }, { "s1", "b0" })
-- on face
constraint("F", { "s0", "b2", "p0" }, { "s1", "b0" })
constraint("F", { "s1", "b4", "p0" }, { "s2", "b0" })
constraint("F", { "s2", "b4", "p0" }, { "s3", "b2" })
constraint("F", { "s3", "b0", "p0" }, { "s4", "b2" })
constraint("F", { "s4", "b0", "p0" }, { "s5", "b4" })
constraint("F", { "s5", "b2", "p0" }, { "s0", "b4" })
-- on walls
constraint("WB", { "s0", "b1", "p0" }, { "s1", "b3" })
constraint("WB", { "s1", "b2", "p0" }, { "s2", "b0" })
constraint("WB", { "s2", "b3", "p0" }, { "s3", "b1" })
constraint("WB", { "s3", "b0", "p0" }, { "s0", "b2" })

constraint("WC", { "s1", "b2", "p0" }, { "s2", "b0" })
constraint("WC", { "s3", "b0", "p0" }, { "s0", "b2" })
-- on volume
constraint("Vol", { "s0", "b1" }, { "s6", "b0" })
constraint("Vol", { "s1", "b1" }, { "s7", "b0" })
constraint("Vol", { "s2", "b1" }, { "s8", "b0" })
constraint("Vol", { "s3", "b1" }, { "s9", "b0" })
constraint("Vol", { "s4", "b1" }, { "s10", "b0" })
constraint("Vol", { "s5", "b1" }, { "s11", "b0" })

constraint("Vol", { "s0", "b4", "p0" }, { "s1", "b2" })
constraint("Vol", { "s1", "b6", "p0" }, { "s2", "b2" })
constraint("Vol", { "s2", "b6", "p0" }, { "s3", "b4" })
constraint("Vol", { "s3", "b2", "p0" }, { "s4", "b4" })
constraint("Vol", { "s4", "b2", "p0" }, { "s5", "b6" })
constraint("Vol", { "s5", "b4", "p0" }, { "s0", "b6" })

constraint("Vol", { "s6", "b4", "p0" }, { "s7", "b2" })
constraint("Vol", { "s7", "b6", "p0" }, { "s8", "b2" })
constraint("Vol", { "s8", "b6", "p0" }, { "s9", "b4" })
constraint("Vol", { "s9", "b2", "p0" }, { "s10", "b4" })
constraint("Vol", { "s10", "b2", "p0" }, { "s11", "b6" })
constraint("Vol", { "s11", "b4", "p0" }, { "s6", "b6" })
-- on incidence operators
constraint("F", { "b0", "b1" }, { "b1", "b0" })
constraint("F", { "b1", "b1" }, { "b2", "b0" })
constraint("F", { "b2", "b1" }, { "b3", "b0" })
constraint("F", { "b3", "b1" }, { "b4", "b0" })
constraint("F", { "b4", "b1" }, { "b5", "b0" })
constraint("F", { "b5", "b1" }, { "b0", "b0" })

constraint("WB", { "b0", "b1" }, { "b1", "b0" })
constraint("WB", { "b1", "b1" }, { "b2", "b0" })
constraint("WB", { "b2", "b1" }, { "b3", "b0" })
constraint("WB", { "b3", "b1" }, { "b0", "b0" })

constraint("WC", { "b0", "b1" }, { "b1", "b0" })
constraint("WC", { "b1", "b1" }, { "b2", "b0" })
constraint("WC", { "b2", "b1" }, { "b3", "b0" })
constraint("WC", { "b3", "b1" }, { "b0", "b0" })

constraint("Vol", { "b0", "b0", "p0" }, { "b2", "b0" })
constraint("Vol", { "b0", "b1", "p0" }, { "b3", "b0" })
constraint("Vol", { "b0", "b2", "p0" }, { "b4", "b0" })
constraint("Vol", { "b0", "b3", "p0" }, { "b5", "b0" })
constraint("Vol", { "b0", "b4", "p0" }, { "b6", "b0" })
constraint("Vol", { "b0", "b5", "p0" }, { "b7", "b0" })

constraint("Vol", { "b1", "b0" }, { "b2", "b2" })
constraint("Vol", { "b1", "b1" }, { "b3", "b2" })
constraint("Vol", { "b1", "b2" }, { "b4", "b2" })
constraint("Vol", { "b1", "b3" }, { "b5", "b2" })
constraint("Vol", { "b1", "b4" }, { "b6", "b2" })
constraint("Vol", { "b1", "b5" }, { "b7", "b2" })

constraint("Vol", { "b2", "b3", "p0" }, { "b3", "b1" })
constraint("Vol", { "b3", "b3", "p0" }, { "b4", "b1" })
constraint("Vol", { "b4", "b3", "p0" }, { "b5", "b1" })
constraint("Vol", { "b5", "b3", "p0" }, { "b6", "b1" })
constraint("Vol", { "b6", "b3", "p0" }, { "b7", "b1" })
constraint("Vol", { "b7", "b3", "p0" }, { "b2", "b1" })

-- init matrices for edges
-- for cantor
initMat("C", { "s0", "b1" }, {
    { 2.0 / 3.0 },
    { 1.0 / 3.0 }
}, "CONST")
-- for bezier
initMat("B", { "s0" }, {
    { 1.0, 0.5, 0.25 },
    { 0.0, 0.5, 0.50 },
    { 0.0, 0.0, 0.25 }
}, "CONST")

-- to have a more consistent boundary
-- initMat("WC", "s0", {
--     { 1, 2.0 / 3.0, 1.0 / 3.0, 0.5 },
--     { 0, 1.0 / 3.0, 1.0 / 6.0, 0.0 },
--     { 0,       0.0, 1.0 / 6.0, 0.0 },
--     { 0,       0.0, 1.0 / 3.0, 0.5 }
-- }, "CONST")
-- initMat("WB", "s0", {
--     { 1, 0.5, 0.25, 0.125, 0.25, 0.5 },
--     { 0, 0.5, 0.50, 0.250, 0.25, 0.0 },
--     { 0, 0.0, 0.25, 0.125, 0.00, 0.0 },
--     { 0, 0.0, 0.00, 0.125, 0.00, 0.0 },
--     { 0, 0.0, 0.00, 0.250, 0.25, 0.0 },
--     { 0, 0.0, 0.00, 0.125, 0.25, 0.5 }
-- }, "CONST")

-- init control points
initMat("init", { "s0" }, {
    {
        3.0 * math.cos(0.0 * math.pi * 2.0 / 6.0),
        3.0 * math.cos(1.0 * math.pi * 2.0 / 6.0),
        3.0 * 0.7 * math.cos(3.0 * math.pi * 2.0 / 12.0),
        3.0 * math.cos(2.0 * math.pi * 2.0 / 6.0),
        3.0 * math.cos(3.0 * math.pi * 2.0 / 6.0),
        3.0 * 0.7 * math.cos(7.0 * math.pi * 2.0 / 12.0),
        3.0 * math.cos(4.0 * math.pi * 2.0 / 6.0),
        3.0 * math.cos(5.0 * math.pi * 2.0 / 6.0),
        3.0 * 0.7 * math.cos(11.0 * math.pi * 2.0 / 12.0),
        3.0 * math.cos(0.0 * math.pi * 2.0 / 6.0),
        3.0 * math.cos(1.0 * math.pi * 2.0 / 6.0),
        3.0 * 0.7 * math.cos(3.0 * math.pi * 2.0 / 12.0),
        3.0 * math.cos(2.0 * math.pi * 2.0 / 6.0),
        3.0 * math.cos(3.0 * math.pi * 2.0 / 6.0),
        3.0 * 0.7 * math.cos(7.0 * math.pi * 2.0 / 12.0),
        3.0 * math.cos(4.0 * math.pi * 2.0 / 6.0),
        3.0 * math.cos(5.0 * math.pi * 2.0 / 6.0),
        3.0 * 0.7 * math.cos(11.0 * math.pi * 2.0 / 12.0)
    },
    {
        3.0 * math.sin(0.0 * math.pi * 2.0 / 6.0),
        3.0 * math.sin(1.0 * math.pi * 2.0 / 6.0),
        3.0 * 0.7 * math.sin(3.0 * math.pi * 2.0 / 12.0),
        3.0 * math.sin(2.0 * math.pi * 2.0 / 6.0),
        3.0 * math.sin(3.0 * math.pi * 2.0 / 6.0),
        3.0 * 0.7 * math.sin(7.0 * math.pi * 2.0 / 12.0),
        3.0 * math.sin(4.0 * math.pi * 2.0 / 6.0),
        3.0 * math.sin(5.0 * math.pi * 2.0 / 6.0),
        3.0 * 0.7 * math.sin(11.0 * math.pi * 2.0 / 12.0),
        3.0 * math.sin(0.0 * math.pi * 2.0 / 6.0),
        3.0 * math.sin(1.0 * math.pi * 2.0 / 6.0),
        3.0 * 0.7 * math.sin(3.0 * math.pi * 2.0 / 12.0),
        3.0 * math.sin(2.0 * math.pi * 2.0 / 6.0),
        3.0 * math.sin(3.0 * math.pi * 2.0 / 6.0),
        3.0 * 0.7 * math.sin(7.0 * math.pi * 2.0 / 12.0),
        3.0 * math.sin(4.0 * math.pi * 2.0 / 6.0),
        3.0 * math.sin(5.0 * math.pi * 2.0 / 6.0),
        3.0 * 0.7 * math.sin(11.0 * math.pi * 2.0 / 12.0)
    },
    { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
      -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0 }
}, "VAR")