-- states
state("V", 1)
state("B", 1)
state("C", 0)
state("F", 0)

-- permutations
permutation("p0", "C", "C")
permutation("p0", "B", "B")

-- boundary of states
boundary("b0", "B", "V")
boundary("b1", "B", "V")
boundary("b0", "C", "V")
boundary("b1", "C", "V")
boundary("b0", "F", "C")
boundary("b1", "F", "B")
boundary("b2", "F", "C")
boundary("b3", "F", "B")
boundary("b4", "F", "C")
boundary("b5", "F", "B")

-- grid of states
grid("F", {
    { { "b0", "b0" }, { "b0", "b1" } },
    { { "b1", "b0" }, { "b1", "intern_0" }, { "b1", "b1" } },
    { { "b2", "b0" }, { "b2", "b1" } },
    { { "b3", "b0" }, { "b3", "intern_0" }, { "b3", "b1" } },
    { { "b4", "b0" }, { "b4", "b1" } },
    { { "b5", "b0" }, { "b5", "intern_0" }, { "b5", "b1" } }
})

-- space of states
space("B", { "b0", "intern_0", "b1" })
space("C", { "b0", "b1" })
space("F", { "b0", "b1", "b2", "b3", "b4", "b5" })

-- subdivision of states
subdivision("s0", "V", "V")
subdivision("s0", "B", "B")
subdivision("s1", "B", "B")
subdivision("s0", "C", "C")
subdivision("s1", "C", "C")
subdivision("s0", "F", "F", { 252/255, 127/255, 0 })
subdivision("s1", "F", "F", { 0, 71/255, 232/255 })
subdivision("s2", "F", "F", { 205/255, 207/255, 0 })
subdivision("s3", "F", "F", { 206/255, 0, 0 })
subdivision("s4", "F", "F", { 0, 183/255, 0 })
subdivision("s5", "F", "F", { 129/255, 50/255, 1 })
subdivision("s0", "init", "F")

-- primitive of states
primitive("F", {
    {
        { "b0", "b0" },
        { "b1", "b0" },
        { "b1", "s0", "s1", "b0" },
        { "b1", "s1", "b0" },
        { "b1", "s1", "s1", "b0" },
        { "b2", "b0" },
        { "b3", "b0" },
        { "b3", "s0", "s1", "b0" },
        { "b3", "s1", "b0" },
        { "b3", "s1", "s1", "b0" },
        { "b4", "b0" },
        { "b5", "b0" },
        { "b5", "s0", "s1", "b0" },
        { "b5", "s1", "b0" },
        { "b5", "s1", "s1", "b0" },
    }
})

-- permutation constraints
-- to define permutation operators
constraint("C", { "p0", "b0" }, { "b1" })
constraint("C", { "p0", "b1" }, { "b0" })
constraint("B", { "p0", "b0" }, { "b1" })
constraint("B", { "p0", "intern_0" }, { "intern_0" })
constraint("B", { "p0", "b1" }, { "b0" })
-- to constrain subdivision operators using permutation operators
constraint("C", { "p0", "s0" }, { "s1", "p0" })
constraint("C", { "p0", "s1" }, { "s0", "p0" })
constraint("B", { "p0", "s0" }, { "s1", "p0" })
constraint("B", { "p0", "s1" }, { "s0", "p0" })

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
-- on incidence operators
constraint("F", { "b0", "b1" }, { "b1", "b0" })
constraint("F", { "b1", "b1" }, { "b2", "b0" })
constraint("F", { "b2", "b1" }, { "b3", "b0" })
constraint("F", { "b3", "b1" }, { "b4", "b0" })
constraint("F", { "b4", "b1" }, { "b5", "b0" })
constraint("F", { "b5", "b1" }, { "b0", "b0" })

-- init matrices for edges
-- for cantor
initMat("C", "s0", {
    { 1.0, 2.0 / 3.0 },
    { 0.0, 1.0 / 3.0 }
}, "CONST")
initMat("C", "s1", {
    { 1.0 / 3.0, 0.0 },
    { 2.0 / 3.0, 1.0 }
}, "CONST")
-- for bezier
initMat("B", "s0", {
    { 1.0, 0.5, 0.25 },
    { 0.0, 0.5, 0.50 },
    { 0.0, 0.0, 0.25 }
}, "CONST")
initMat("B", "s1", {
    { 0.25, 0.0, 0.0 },
    { 0.50, 0.5, 0.0 },
    { 0.25, 0.5, 1.0 }
}, "CONST")

-- init control points
initMat("init", "s0", {
    {
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
        3.0 * 0.7 * math.sin(11.0 * math.pi * 2.0 / 12.0)
    },
    { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }
}, "VAR")