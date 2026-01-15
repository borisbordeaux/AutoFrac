-- states
state("V", 1)
state("B", 1)
state("C", 0)
state("F", 0)

-- permutations
permutation("permutC", "C", "C")
permutation("permutB", "B", "B")

-- boundary of states
boundary("b0bezier", "B", "V")
boundary("b1bezier", "B", "V")
boundary("b0cantor", "C", "V")
boundary("b1cantor", "C", "V")
boundary("b0face", "F", "C")
boundary("b1face", "F", "B")
boundary("b2face", "F", "C")
boundary("b3face", "F", "B")
boundary("b4face", "F", "C")
boundary("b5face", "F", "B")

-- grid of states
grid("F", {
    { { "b0face", "b0cantor" }, { "b0face", "b1cantor" } },
    { { "b1face", "b0bezier" }, { "b1face", "B_intern_0" }, { "b1face", "b1bezier" } },
    { { "b2face", "b0cantor" }, { "b2face", "b1cantor" } },
    { { "b3face", "b0bezier" }, { "b3face", "B_intern_0" }, { "b3face", "b1bezier" } },
    { { "b4face", "b0cantor" }, { "b4face", "b1cantor" } },
    { { "b5face", "b0bezier" }, { "b5face", "B_intern_0" }, { "b5face", "b1bezier" } }
})

-- space of states
space("B", { "b0bezier", "B_intern_0", "b1bezier" })
space("C", { "b0cantor", "b1cantor" })
space("F", { "b0face", "b1face", "b2face", "b3face", "b4face", "b5face" })

-- subdivision of states
subdivision("s0vert", "V", "V")
subdivision("s0bezier", "B", "B")
subdivision("s1bezier", "B", "B")
subdivision("s0cantor", "C", "C")
subdivision("s1cantor", "C", "C")
subdivision("s0face", "F", "F")
subdivision("s1face", "F", "F")
subdivision("s2face", "F", "F")
subdivision("s3face", "F", "F")
subdivision("s4face", "F", "F")
subdivision("s5face", "F", "F")
subdivision("s0init", "init", "F")

-- primitive of states
primitive("F", {
    {
        { "b0face", "b0cantor" },
        { "b1face", "b0bezier" },
        { "b1face", "s0bezier", "s1bezier", "b0bezier" },
        { "b1face", "s1bezier", "b0bezier" },
        { "b1face", "s1bezier", "s1bezier", "b0bezier" },
        { "b2face", "b0cantor" },
        { "b3face", "b0bezier" },
        { "b3face", "s0bezier", "s1bezier", "b0bezier" },
        { "b3face", "s1bezier", "b0bezier" },
        { "b3face", "s1bezier", "s1bezier", "b0bezier" },
        { "b4face", "b0cantor" },
        { "b5face", "b0bezier" },
        { "b5face", "s0bezier", "s1bezier", "b0bezier" },
        { "b5face", "s1bezier", "b0bezier" },
        { "b5face", "s1bezier", "s1bezier", "b0bezier" },
    }
})

-- permutation constraints
-- to define permutation operators
constraint({ "permutC", "b0cantor" }, { "b1cantor" })
constraint({ "permutC", "b1cantor" }, { "b0cantor" })
constraint({ "permutB", "b0bezier" }, { "b1bezier" })
constraint({ "permutB", "B_intern_0" }, { "B_intern_0" })
constraint({ "permutB", "b1bezier" }, { "b0bezier" })
-- to constrain subdivision operators using permutation operators
constraint({ "permutC", "s0cantor" }, { "s1cantor", "permutC" })
constraint({ "permutC", "s1cantor" }, { "s0cantor", "permutC" })
constraint({ "permutB", "s0bezier" }, { "s1bezier", "permutB" })
constraint({ "permutB", "s1bezier" }, { "s0bezier", "permutB" })

-- incidence constraints
-- on edge
constraint({ "b0cantor", "s0vert" }, { "s0cantor", "b0cantor" })
constraint({ "b1cantor", "s0vert" }, { "s1cantor", "b1cantor" })
constraint({ "b0bezier", "s0vert" }, { "s0bezier", "b0bezier" })
constraint({ "b1bezier", "s0vert" }, { "s1bezier", "b1bezier" })
-- on face
constraint({ "b0face", "s0cantor" }, { "s5face", "b0face" })
constraint({ "b0face", "s1cantor" }, { "s0face", "b0face" })
constraint({ "b1face", "s0bezier" }, { "s0face", "b1face" })
constraint({ "b1face", "s1bezier" }, { "s1face", "b1face" })
constraint({ "b2face", "s0cantor" }, { "s1face", "b2face" })
constraint({ "b2face", "s1cantor" }, { "s2face", "b2face" })
constraint({ "b3face", "s0bezier" }, { "s2face", "b3face" })
constraint({ "b3face", "s1bezier" }, { "s3face", "b3face" })
constraint({ "b4face", "s0cantor" }, { "s3face", "b4face" })
constraint({ "b4face", "s1cantor" }, { "s4face", "b4face" })
constraint({ "b5face", "s0bezier" }, { "s4face", "b5face" })
constraint({ "b5face", "s1bezier" }, { "s5face", "b5face" })

-- adjacency constraints
-- on edge
constraint({ "s0bezier", "b1bezier" }, { "s1bezier", "b0bezier" })
-- on face
constraint({ "s0face", "b2face", "permutC" }, { "s1face", "b0face" })
constraint({ "s1face", "b4face", "permutC" }, { "s2face", "b0face" })
constraint({ "s2face", "b4face", "permutC" }, { "s3face", "b2face" })
constraint({ "s3face", "b0face", "permutC" }, { "s4face", "b2face" })
constraint({ "s4face", "b0face", "permutC" }, { "s5face", "b4face" })
constraint({ "s5face", "b2face", "permutC" }, { "s0face", "b4face" })
-- on incidence operators
constraint({ "b0face", "b1cantor" }, { "b1face", "b0bezier" })
constraint({ "b1face", "b1bezier" }, { "b2face", "b0cantor" })
constraint({ "b2face", "b1cantor" }, { "b3face", "b0bezier" })
constraint({ "b3face", "b1bezier" }, { "b4face", "b0cantor" })
constraint({ "b4face", "b1cantor" }, { "b5face", "b0bezier" })
constraint({ "b5face", "b1bezier" }, { "b0face", "b0cantor" })

-- init matrices for edges
-- for cantor
initMat("s0cantor", {
    { 1.0, 2.0 / 3.0 },
    { 0.0, 1.0 / 3.0 }
}, "CONST")
initMat("s1cantor", {
    { 1.0 / 3.0, 0.0 },
    { 2.0 / 3.0, 1.0 }
}, "CONST")
-- for bezier
initMat("s0bezier", {
    { 1.0, 0.5, 0.25 },
    { 0.0, 0.5, 0.50 },
    { 0.0, 0.0, 0.25 }
}, "CONST")
initMat("s1bezier", {
    { 0.25, 0.0, 0.0 },
    { 0.50, 0.5, 0.0 },
    { 0.25, 0.5, 1.0 }
}, "CONST")

-- init control points
initMat("s0init", {
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