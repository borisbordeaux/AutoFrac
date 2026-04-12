-- states
state("V", 1)
state("E", 0)
state("F", 0)

-- permutations
permutation("p0", "E", "E")

-- boundary of states
boundary("b0", "E", "V")
boundary("b1", "E", "V")

boundary("b0", "F", "E")
boundary("b1", "F", "E")
boundary("b2", "F", "E")
boundary("b3", "F", "E")

-- grid of states
grid("E", {
    { { "b0" }, { "b1" } }
})

gridFromBoundary("F")

-- space of states
space("E", { "b0", "b1" })
space("F", { "b0", "b1", "b2", "b3" })

-- subdivision of states
subdivision("s0", "V", "V")

subdivision("s0", "E", "E")
subdivision("s1", "E", "E")

local red = { 0.7, 0.3, 0.3 }
local green = { 0.3, 0.7, 0.3 }
local blue = { 0.3, 0.3, 0.7 }

subdivision("s0", "F", "F", red)
subdivision("s1", "F", "F", green)
subdivision("s2", "F", "F", blue)
subdivision("s3", "F", "F")

subdivision("s0", "init", "F")

-- primitive of states
-- instead of defining a primitive with boundary operators like this:
-- primitive("F", {
--     {
--         { "b0", "b0" },
--         { "b1", "b0" },
--         { "b2", "b0" },
--         { "b3", "b0" }
--     }
-- })
-- we can define a primitive with a set of matrices, allowing
-- to define an arbitrary shape using barycentric coordinates
primitiveMat("F", {
    {
        { 2/3, 1/3, 0.0, 0.0, 0.0, 0.0, 1/3, 2/3 },
        { 1/3, 2/3, 2/3, 1/3, 0.0, 0.0, 0.0, 0.0 },
        { 0.0, 0.0, 1/3, 2/3, 2/3, 1/3, 0.0, 0.0 },
        { 0.0, 0.0, 0.0, 0.0, 1/3, 2/3, 2/3, 1/3 }
    },
    {
        { 1.0, 2/3, 2/3 },
        { 0.0, 0.0, 1/3 },
        { 0.0, 0.0, 0.0 },
        { 0.0, 1/3, 0.0 }
    },
    {
        { 0.0, 1/3, 0.0 },
        { 1.0, 2/3, 2/3 },
        { 0.0, 0.0, 1/3 },
        { 0.0, 0.0, 0.0 }
    },
    {
        { 0.0, 0.0, 0.0 },
        { 0.0, 1/3, 0.0 },
        { 1.0, 2/3, 2/3 },
        { 0.0, 0.0, 1/3 }
    },
    {
        { 0.0, 0.0, 1/3 },
        { 0.0, 0.0, 0.0 },
        { 0.0, 1/3, 0.0 },
        { 1.0, 2/3, 2/3 }
    }
})

-- permutation constraints
-- to define permutation operators
constraint("E", { "p0", "b0" }, { "b1" })
constraint("E", { "p0", "b1" }, { "b0" })

-- to constrain subdivision operators using permutation operators
constraint("E", { "p0", "s0" }, { "s1", "p0" })
constraint("E", { "p0", "s1" }, { "s0", "p0" })

-- incidence constraints
-- on edge
constraint("E", { "b0", "s0" }, { "s0", "b0" })
constraint("E", { "b1", "s0" }, { "s1", "b1" })
-- on face
constraint("F", { "b0", "s0" }, { "s0", "b0" })
constraint("F", { "b0", "s1" }, { "s1", "b0" })
constraint("F", { "b1", "s0" }, { "s1", "b1" })
constraint("F", { "b1", "s1" }, { "s2", "b1" })
constraint("F", { "b2", "s0" }, { "s2", "b2" })
constraint("F", { "b2", "s1" }, { "s3", "b2" })
constraint("F", { "b3", "s0" }, { "s3", "b3" })
constraint("F", { "b3", "s1" }, { "s0", "b3" })

-- adjacency constraints
-- on edge
constraint("E", { "s0", "b1" }, { "s1", "b0" })
-- on face
constraint("F", { "s0", "b1", "p0" }, { "s1", "b3" })
constraint("F", { "s1", "b2", "p0" }, { "s2", "b0" })
constraint("F", { "s2", "b3", "p0" }, { "s3", "b1" })
constraint("F", { "s3", "b0", "p0" }, { "s0", "b2" })

-- on incidence operators for face
constraint("F", { "b0", "b1" }, { "b1", "b0" })
constraint("F", { "b1", "b1" }, { "b2", "b0" })
constraint("F", { "b2", "b1" }, { "b3", "b0" })
constraint("F", { "b3", "b1" }, { "b0", "b0" })

local f = 3

initMat("init", { "s0" }, {
    { -0.5 * f, -0.5 * f,  0.5 * f,  0.5 * f },
    {  0.5 * f, -0.5 * f, -0.5 * f,  0.5 * f },
    {  0      ,  0      ,  0      ,  0       }
}, "VAR")