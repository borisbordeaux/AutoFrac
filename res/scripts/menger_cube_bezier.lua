-- states
state("V", 1)
state("E", 1)
state("F", 0)
state("C", 0)

-- permutations
permutation("p0", "E", "E")
permutation("p0", "F", "F")
permutation("r0", "F", "F")

-- boundary of states
boundary("b0", "E", "V")
boundary("b1", "E", "V")

boundary("b0", "F", "E")
boundary("b1", "F", "E")
boundary("b2", "F", "E")
boundary("b3", "F", "E")

boundary("b0", "C", "F")
boundary("b1", "C", "F")
boundary("b2", "C", "F")
boundary("b3", "C", "F")
boundary("b4", "C", "F")
boundary("b5", "C", "F")

-- grid of states
grid("E", {
    { { "b0" }, { "intern_0" }, { "b1" } }
})

grid("F", {
    { { "b0", "b0" }, { "b0", "intern_0" }, { "b0", "b1" } },
    { { "b1", "b0" }, { "b1", "intern_0" }, { "b1", "b1" } },
    { { "b2", "b0" }, { "b2", "intern_0" }, { "b2", "b1" } },
    { { "b3", "b0" }, { "b3", "intern_0" }, { "b3", "b1" } }
})

grid("C", {
    { { "b0", "b0", "b0" }, { "b0", "b0", "intern_0" }, { "b0", "b0", "b1" } },
    { { "b0", "b1", "b0" }, { "b0", "b1", "intern_0" }, { "b0", "b1", "b1" } },
    { { "b0", "b2", "b0" }, { "b0", "b2", "intern_0" }, { "b0", "b2", "b1" } },
    { { "b0", "b3", "b0" }, { "b0", "b3", "intern_0" }, { "b0", "b3", "b1" } },
    { { "b1", "b0", "b0" }, { "b1", "b0", "intern_0" }, { "b1", "b0", "b1" } },
    { { "b1", "b1", "b0" }, { "b1", "b1", "intern_0" }, { "b1", "b1", "b1" } },
    { { "b1", "b2", "b0" }, { "b1", "b2", "intern_0" }, { "b1", "b2", "b1" } },
    { { "b1", "b3", "b0" }, { "b1", "b3", "intern_0" }, { "b1", "b3", "b1" } },
    { { "b2", "b0", "b0" }, { "b2", "b0", "intern_0" }, { "b2", "b0", "b1" } },
    { { "b2", "b1", "b0" }, { "b2", "b1", "intern_0" }, { "b2", "b1", "b1" } },
    { { "b2", "b2", "b0" }, { "b2", "b2", "intern_0" }, { "b2", "b2", "b1" } },
    { { "b2", "b3", "b0" }, { "b2", "b3", "intern_0" }, { "b2", "b3", "b1" } },
    { { "b3", "b0", "b0" }, { "b3", "b0", "intern_0" }, { "b3", "b0", "b1" } },
    { { "b3", "b1", "b0" }, { "b3", "b1", "intern_0" }, { "b3", "b1", "b1" } },
    { { "b3", "b2", "b0" }, { "b3", "b2", "intern_0" }, { "b3", "b2", "b1" } },
    { { "b3", "b3", "b0" }, { "b3", "b3", "intern_0" }, { "b3", "b3", "b1" } },
    { { "b4", "b0", "b0" }, { "b4", "b0", "intern_0" }, { "b4", "b0", "b1" } },
    { { "b4", "b1", "b0" }, { "b4", "b1", "intern_0" }, { "b4", "b1", "b1" } },
    { { "b4", "b2", "b0" }, { "b4", "b2", "intern_0" }, { "b4", "b2", "b1" } },
    { { "b4", "b3", "b0" }, { "b4", "b3", "intern_0" }, { "b4", "b3", "b1" } },
    { { "b5", "b0", "b0" }, { "b5", "b0", "intern_0" }, { "b5", "b0", "b1" } },
    { { "b5", "b1", "b0" }, { "b5", "b1", "intern_0" }, { "b5", "b1", "b1" } },
    { { "b5", "b2", "b0" }, { "b5", "b2", "intern_0" }, { "b5", "b2", "b1" } },
    { { "b5", "b3", "b0" }, { "b5", "b3", "intern_0" }, { "b5", "b3", "b1" } },
})

-- space of states
space("E", { "b0", "intern_0", "b1" })
space("F", { "b0", "b1", "b2", "b3" })
space("C", { "b0", "b1", "b2", "b3", "b4", "b5" })

-- subdivision of states
subdivision("s0", "V", "V")

subdivision("s0", "E", "E")
subdivision("s1", "E", "E")
subdivision("s2", "E", "E")

for i=0,7,1 do
    subdivision("s" .. i, "F", "F")
end

local red = { 0.7, 0.3, 0.3 }
local green = { 0.3, 0.7, 0.3 }
local blue = { 0.3, 0.3, 0.7 }
local colors = {
    blue, red, blue, green, blue, red, blue, green,
    green, green, green, green,
    blue, red, blue, green, blue, red, blue, green
}

for i=0,19,1 do
    subdivision("s" .. i, "C", "C", colors[i+1])
end

subdivision("s0", "init", "C")

-- primitive of states
primitive("F", {
    {
        { "b0", "b0" },
        { "b0", "intern_0" },
        { "b1", "b0" },
        { "b1", "intern_0" },
        { "b2", "b0" },
        { "b2", "intern_0" },
        { "b3", "b0" },
        { "b3", "intern_0" }
    }
})

primitive("C", {
    {
        { "b0", "b0", "b0" },
        { "b0", "b0", "intern_0" },
        { "b0", "b1", "b0" },
        { "b0", "b1", "intern_0" },
        { "b0", "b2", "b0" },
        { "b0", "b2", "intern_0" },
        { "b0", "b3", "b0" },
        { "b0", "b3", "intern_0" }
    },
    {
        { "b1", "b0", "b0" },
        { "b1", "b0", "intern_0" },
        { "b1", "b1", "b0" },
        { "b1", "b1", "intern_0" },
        { "b1", "b2", "b0" },
        { "b1", "b2", "intern_0" },
        { "b1", "b3", "b0" },
        { "b1", "b3", "intern_0" }
    },
    {
        { "b2", "b0", "b0" },
        { "b2", "b0", "intern_0" },
        { "b2", "b1", "b0" },
        { "b2", "b1", "intern_0" },
        { "b2", "b2", "b0" },
        { "b2", "b2", "intern_0" },
        { "b2", "b3", "b0" },
        { "b2", "b3", "intern_0" }
    },
    {
        { "b3", "b0", "b0" },
        { "b3", "b0", "intern_0" },
        { "b3", "b1", "b0" },
        { "b3", "b1", "intern_0" },
        { "b3", "b2", "b0" },
        { "b3", "b2", "intern_0" },
        { "b3", "b3", "b0" },
        { "b3", "b3", "intern_0" }
    },
    {
        { "b4", "b0", "b0" },
        { "b4", "b0", "intern_0" },
        { "b4", "b1", "b0" },
        { "b4", "b1", "intern_0" },
        { "b4", "b2", "b0" },
        { "b4", "b2", "intern_0" },
        { "b4", "b3", "b0" },
        { "b4", "b3", "intern_0" }
    },
    {
        { "b5", "b0", "b0" },
        { "b5", "b0", "intern_0" },
        { "b5", "b1", "b0" },
        { "b5", "b1", "intern_0" },
        { "b5", "b2", "b0" },
        { "b5", "b2", "intern_0" },
        { "b5", "b3", "b0" },
        { "b5", "b3", "intern_0" }
    },
})

-- permutation constraints
-- to define permutation operators
constraint("E", { "p0", "b0" }, { "b1" })
constraint("E", { "p0", "intern_0" }, { "intern_0" })
constraint("E", { "p0", "b1" }, { "b0" })

constraint("F", { "p0", "b0" }, { "b0", "p0" })
constraint("F", { "p0", "b1" }, { "b3", "p0" })
constraint("F", { "p0", "b2" }, { "b2", "p0" })
constraint("F", { "p0", "b3" }, { "b1", "p0" })

constraint("F", { "r0", "b0" }, { "b1" })
constraint("F", { "r0", "b1" }, { "b2" })
constraint("F", { "r0", "b2" }, { "b3" })
constraint("F", { "r0", "b3" }, { "b0" })
-- to constrain subdivision operators using permutation operators
constraint("E", { "p0", "s0" }, { "s2", "p0" })
constraint("E", { "p0", "s1" }, { "s1", "p0" })
constraint("E", { "p0", "s2" }, { "s0", "p0" })

constraint("F", { "p0", "s0" }, { "s2", "p0" })
constraint("F", { "p0", "s1" }, { "s1", "p0" })
constraint("F", { "p0", "s2" }, { "s0", "p0" })
constraint("F", { "p0", "s3" }, { "s7", "p0" })
constraint("F", { "p0", "s4" }, { "s6", "p0" })
constraint("F", { "p0", "s5" }, { "s5", "p0" })
constraint("F", { "p0", "s6" }, { "s4", "p0" })
constraint("F", { "p0", "s7" }, { "s3", "p0" })

constraint("F", { "r0", "s0" }, { "s2", "r0" })
constraint("F", { "r0", "s1" }, { "s3", "r0" })
constraint("F", { "r0", "s2" }, { "s4", "r0" })
constraint("F", { "r0", "s3" }, { "s5", "r0" })
constraint("F", { "r0", "s4" }, { "s6", "r0" })
constraint("F", { "r0", "s5" }, { "s7", "r0" })
constraint("F", { "r0", "s6" }, { "s0", "r0" })
constraint("F", { "r0", "s7" }, { "s1", "r0" })

-- incidence constraints
-- on edge
constraint("E", { "b0", "s0" }, { "s0", "b0" })
constraint("E", { "b1", "s0" }, { "s2", "b1" })
-- on face
constraint("F", { "b0", "s0" }, { "s0", "b0" })
constraint("F", { "b0", "s1" }, { "s1", "b0" })
constraint("F", { "b0", "s2" }, { "s2", "b0" })
constraint("F", { "b1", "s0" }, { "s2", "b1" })
constraint("F", { "b1", "s1" }, { "s3", "b1" })
constraint("F", { "b1", "s2" }, { "s4", "b1" })
constraint("F", { "b2", "s0" }, { "s4", "b2" })
constraint("F", { "b2", "s1" }, { "s5", "b2" })
constraint("F", { "b2", "s2" }, { "s6", "b2" })
constraint("F", { "b3", "s0" }, { "s6", "b3" })
constraint("F", { "b3", "s1" }, { "s7", "b3" })
constraint("F", { "b3", "s2" }, { "s0", "b3" })
-- on cube
constraint("C", { "b0", "s0" }, { "s0", "b0" })
constraint("C", { "b0", "s1" }, { "s1", "b0" })
constraint("C", { "b0", "s2" }, { "s2", "b0" })
constraint("C", { "b0", "s3" }, { "s3", "b0" })
constraint("C", { "b0", "s4" }, { "s4", "b0" })
constraint("C", { "b0", "s5" }, { "s5", "b0" })
constraint("C", { "b0", "s6" }, { "s6", "b0" })
constraint("C", { "b0", "s7" }, { "s7", "b0" })

constraint("C", { "b1", "s0" }, { "s6", "b1" })
constraint("C", { "b1", "s1" }, { "s5", "b1" })
constraint("C", { "b1", "s2" }, { "s4", "b1" })
constraint("C", { "b1", "s3" }, { "s10", "b1" })
constraint("C", { "b1", "s4" }, { "s16", "b1" })
constraint("C", { "b1", "s5" }, { "s17", "b1" })
constraint("C", { "b1", "s6" }, { "s18", "b1" })
constraint("C", { "b1", "s7" }, { "s11", "b1" })

constraint("C", { "b2", "s0" }, { "s0", "b2" })
constraint("C", { "b2", "s1" }, { "s7", "b2" })
constraint("C", { "b2", "s2" }, { "s6", "b2" })
constraint("C", { "b2", "s3" }, { "s11", "b2" })
constraint("C", { "b2", "s4" }, { "s18", "b2" })
constraint("C", { "b2", "s5" }, { "s19", "b2" })
constraint("C", { "b2", "s6" }, { "s12", "b2" })
constraint("C", { "b2", "s7" }, { "s8", "b2" })

constraint("C", { "b3", "s0" }, { "s12", "b3" })
constraint("C", { "b3", "s1" }, { "s13", "b3" })
constraint("C", { "b3", "s2" }, { "s14", "b3" })
constraint("C", { "b3", "s3" }, { "s9", "b3" })
constraint("C", { "b3", "s4" }, { "s2", "b3" })
constraint("C", { "b3", "s5" }, { "s1", "b3" })
constraint("C", { "b3", "s6" }, { "s0", "b3" })
constraint("C", { "b3", "s7" }, { "s8", "b3" })

constraint("C", { "b4", "s0" }, { "s4", "b4" })
constraint("C", { "b4", "s1" }, { "s3", "b4" })
constraint("C", { "b4", "s2" }, { "s2", "b4" })
constraint("C", { "b4", "s3" }, { "s9", "b4" })
constraint("C", { "b4", "s4" }, { "s14", "b4" })
constraint("C", { "b4", "s5" }, { "s15", "b4" })
constraint("C", { "b4", "s6" }, { "s16", "b4" })
constraint("C", { "b4", "s7" }, { "s10", "b4" })

constraint("C", { "b5", "s0" }, { "s18", "b5" })
constraint("C", { "b5", "s1" }, { "s17", "b5" })
constraint("C", { "b5", "s2" }, { "s16", "b5" })
constraint("C", { "b5", "s3" }, { "s15", "b5" })
constraint("C", { "b5", "s4" }, { "s14", "b5" })
constraint("C", { "b5", "s5" }, { "s13", "b5" })
constraint("C", { "b5", "s6" }, { "s12", "b5" })
constraint("C", { "b5", "s7" }, { "s19", "b5" })

-- adjacency constraints
-- on edge
constraint("E", { "s0", "b1" }, { "s1", "b0" })
constraint("E", { "s1", "b1" }, { "s2", "b0" })
-- on face
constraint("F", { "s0", "b1", "p0" }, { "s1", "b3" })
constraint("F", { "s1", "b1", "p0" }, { "s2", "b3" })
constraint("F", { "s2", "b2", "p0" }, { "s3", "b0" })
constraint("F", { "s3", "b2", "p0" }, { "s4", "b0" })
constraint("F", { "s4", "b3", "p0" }, { "s5", "b1" })
constraint("F", { "s5", "b3", "p0" }, { "s6", "b1" })
constraint("F", { "s6", "b0", "p0" }, { "s7", "b2" })
constraint("F", { "s7", "b0", "p0" }, { "s0", "b2" })
-- on cube
constraint("C", { "s0", "b1", "p0", "r0", "r0" }, { "s7", "b3" })
constraint("C", { "s0", "b4", "p0" }, { "s1", "b2" })
constraint("C", { "s0", "b5", "p0", "r0", "r0" }, { "s8", "b0" })
constraint("C", { "s2", "b1", "p0", "r0", "r0" }, { "s3", "b3" })
constraint("C", { "s2", "b2", "p0" }, { "s1", "b4" })
constraint("C", { "s2", "b5", "p0", "r0", "r0" }, { "s9", "b0" })
constraint("C", { "s4", "b2", "p0" }, { "s5", "b4" })
constraint("C", { "s4", "b3", "p0", "r0", "r0" }, { "s3", "b1" })
constraint("C", { "s4", "b5", "p0", "r0", "r0" }, { "s10", "b0" })
constraint("C", { "s6", "b3", "p0", "r0", "r0" }, { "s7", "b1" })
constraint("C", { "s6", "b4", "p0" }, { "s5", "b2" })
constraint("C", { "s6", "b5", "p0", "r0", "r0" }, { "s11", "b0" })
constraint("C", { "s12", "b0", "p0", "r0", "r0" }, { "s8", "b5" })
constraint("C", { "s12", "b1", "p0", "r0", "r0" }, { "s19", "b3" })
constraint("C", { "s12", "b4", "p0" }, { "s13", "b2" })
constraint("C", { "s14", "b0", "p0", "r0", "r0" }, { "s9", "b5" })
constraint("C", { "s14", "b1", "p0", "r0", "r0" }, { "s15", "b3" })
constraint("C", { "s14", "b2", "p0" }, { "s13", "b4" })
constraint("C", { "s16", "b0", "p0", "r0", "r0" }, { "s10", "b5" })
constraint("C", { "s16", "b2", "p0" }, { "s17", "b4" })
constraint("C", { "s16", "b3", "p0", "r0", "r0" }, { "s15", "b1" })
constraint("C", { "s18", "b0", "p0", "r0", "r0" }, { "s11", "b5" })
constraint("C", { "s18", "b3", "p0", "r0", "r0" }, { "s19", "b1" })
constraint("C", { "s18", "b4", "p0" }, { "s17", "b2" })
-- on incidence operators for face
constraint("F", { "b0", "b1" }, { "b1", "b0" })
constraint("F", { "b1", "b1" }, { "b2", "b0" })
constraint("F", { "b2", "b1" }, { "b3", "b0" })
constraint("F", { "b3", "b1" }, { "b0", "b0" })
-- on incidence operators for cube
constraint("C", { "b0", "b0", "p0" }, { "b3", "b2" })
constraint("C", { "b0", "b1", "p0" }, { "b4", "b0" })
constraint("C", { "b0", "b2", "p0" }, { "b1", "b0" })
constraint("C", { "b0", "b3", "p0" }, { "b2", "b0" })
constraint("C", { "b5", "b0", "p0" }, { "b1", "b2" })
constraint("C", { "b5", "b1", "p0" }, { "b4", "b2" })
constraint("C", { "b5", "b2", "p0" }, { "b3", "b0" })
constraint("C", { "b5", "b3", "p0" }, { "b2", "b2" })
constraint("C", { "b1", "b3", "p0" }, { "b2", "b1" })
constraint("C", { "b2", "b3", "p0" }, { "b3", "b3" })
constraint("C", { "b3", "b1", "p0" }, { "b4", "b1" })
constraint("C", { "b4", "b3", "p0" }, { "b1", "b1" })

-- init matrices for edges (bezier 3)
initMat("E", "s0", {
    { 1.0000, 0.6667, 0.4444 },
    { 0.0000, 0.3333, 0.4444 },
    { 0.0000, 0.0000, 0.1111 }
}, "CONST")
initMat("E", "s1", {
    { 0.4444, 0.2222, 0.1111 },
    { 0.4444, 0.5556, 0.4444 },
    { 0.1111, 0.2222, 0.4444 }
}, "CONST")
initMat("E", "s2", {
    { 0.1111, 0.0000, 0.0000 },
    { 0.4444, 0.3333, 0.0000 },
    { 0.4444, 0.6667, 1.0000 }
}, "CONST")