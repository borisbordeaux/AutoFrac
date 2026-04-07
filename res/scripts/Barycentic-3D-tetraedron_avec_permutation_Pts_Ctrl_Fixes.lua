-- declaration des etats
state('v', 0)
state('f', 6) -- les 3 pts de ctrl internes sur les faces donnent 24 pts ctrl sur le volume

-- transitions
subdivision('init_0', 'init', 'v')

-- on part d'un volume a 6 faces
boundary('v_R', 'v', 'f')
boundary('v_V', 'v', 'f')
boundary('v_B', 'v', 'f')
boundary('v_J', 'v', 'f')

subdivision('v_R0', 'v', 'v', { 0.7, 0, 0 })
subdivision('v_R1', 'v', 'v', { 0.7, 0, 0 })
subdivision('v_R2', 'v', 'v', { 0.7, 0, 0 })
subdivision('v_R3', 'v', 'v', { 0.7, 0, 0 })
subdivision('v_R4', 'v', 'v', { 0.7, 0, 0 })
subdivision('v_R5', 'v', 'v', { 0.7, 0, 0 })

subdivision('v_V0', 'v', 'v', { 0, 0.4, 0 })
subdivision('v_V1', 'v', 'v', { 0, 0.4, 0 })
subdivision('v_V2', 'v', 'v', { 0, 0.4, 0 })
subdivision('v_V3', 'v', 'v', { 0, 0.4, 0 })
subdivision('v_V4', 'v', 'v', { 0, 0.4, 0 })
subdivision('v_V5', 'v', 'v', { 0, 0.4, 0 })

subdivision('v_B0', 'v', 'v', { 0, 0, 0.7 })
subdivision('v_B1', 'v', 'v', { 0, 0, 0.7 })
subdivision('v_B2', 'v', 'v', { 0, 0, 0.7 })
subdivision('v_B3', 'v', 'v', { 0, 0, 0.7 })
subdivision('v_B4', 'v', 'v', { 0, 0, 0.7 })
subdivision('v_B5', 'v', 'v', { 0, 0, 0.7 })

subdivision('v_J0', 'v', 'v', { 0.8, 0.8, 0 })
subdivision('v_J1', 'v', 'v', { 0.8, 0.8, 0 })
subdivision('v_J2', 'v', 'v', { 0.8, 0.8, 0 })
subdivision('v_J3', 'v', 'v', { 0.8, 0.8, 0 })
subdivision('v_J4', 'v', 'v', { 0.8, 0.8, 0 })
subdivision('v_J5', 'v', 'v', { 0.8, 0.8, 0 })

subdivision('f_0', 'f', 'f')
subdivision('f_1', 'f', 'f')
subdivision('f_2', 'f', 'f')
subdivision('f_3', 'f', 'f')
subdivision('f_4', 'f', 'f')
subdivision('f_5', 'f', 'f')

-- ordonnancement des points de controle
space("f", { 'intern_0', 'intern_1', 'intern_2', 'intern_3', 'intern_4', 'intern_5' })
space("v", { 'v_R', 'v_V', 'v_B', 'v_J' })

-- contraintes topologiques
---------------------- symetrie de la face
permutation("f_rot", "f", "f")
permutation("f_inv", "f", "f")

-- definition de la la permutation rotation
constraint("f", { "f_rot", "intern_0" }, { "intern_2" })
constraint("f", { "f_rot", "intern_1" }, { "intern_3" })
constraint("f", { "f_rot", "intern_2" }, { "intern_4" })
constraint("f", { "f_rot", "intern_3" }, { "intern_5" })
constraint("f", { "f_rot", "intern_4" }, { "intern_0" })
constraint("f", { "f_rot", "intern_5" }, { "intern_1" })

--application des contraintes de symetrie par rotation
constraint("f", { "f_rot", "f_0" }, { "f_2", "f_rot" })
constraint("f", { "f_rot", "f_1" }, { "f_3", "f_rot" })
constraint("f", { "f_rot", "f_2" }, { "f_4", "f_rot" })
constraint("f", { "f_rot", "f_3" }, { "f_5", "f_rot" })
constraint("f", { "f_rot", "f_4" }, { "f_0", "f_rot" })
constraint("f", { "f_rot", "f_5" }, { "f_1", "f_rot" })

-- -- definition de la symetrie
constraint("f", { "f_inv", "intern_0" }, { "intern_5" })
constraint("f", { "f_inv", "intern_1" }, { "intern_4" })
constraint("f", { "f_inv", "intern_2" }, { "intern_3" })
constraint("f", { "f_inv", "intern_3" }, { "intern_2" })
constraint("f", { "f_inv", "intern_4" }, { "intern_1" })
constraint("f", { "f_inv", "intern_5" }, { "intern_0" })

-- application des contraintes de symetrie
constraint("f", { "f_inv", "f_0" }, { "f_5", "f_inv" })
constraint("f", { "f_inv", "f_1" }, { "f_4", "f_inv" })
constraint("f", { "f_inv", "f_2" }, { "f_3", "f_inv" })
constraint("f", { "f_inv", "f_3" }, { "f_2", "f_inv" })
constraint("f", { "f_inv", "f_4" }, { "f_1", "f_inv" })
constraint("f", { "f_inv", "f_5" }, { "f_0", "f_inv" })

-- ------------------------------------------
-- -- volume
-- -------------       -----------------------------
-- -- Contraintes incidences pour v
constraint('v', { 'v_R0', 'v_R' }, { 'v_R', 'f_0' })
constraint('v', { 'v_R1', 'v_R' }, { 'v_R', 'f_1' })
constraint('v', { 'v_R2', 'v_R' }, { 'v_R', 'f_2' })
constraint('v', { 'v_R3', 'v_R' }, { 'v_R', 'f_3' })
constraint('v', { 'v_R4', 'v_R' }, { 'v_R', 'f_4' })
constraint('v', { 'v_R5', 'v_R' }, { 'v_R', 'f_5' })

constraint('v', { 'v_V0', 'v_V' }, { 'v_V', 'f_0' })
constraint('v', { 'v_V1', 'v_V' }, { 'v_V', 'f_1' })
constraint('v', { 'v_V2', 'v_V' }, { 'v_V', 'f_2' })
constraint('v', { 'v_V3', 'v_V' }, { 'v_V', 'f_3' })
constraint('v', { 'v_V4', 'v_V' }, { 'v_V', 'f_4' })
constraint('v', { 'v_V5', 'v_V' }, { 'v_V', 'f_5' })

constraint('v', { 'v_B0', 'v_B' }, { 'v_B', 'f_0' })
constraint('v', { 'v_B1', 'v_B' }, { 'v_B', 'f_1' })
constraint('v', { 'v_B2', 'v_B' }, { 'v_B', 'f_2' })
constraint('v', { 'v_B3', 'v_B' }, { 'v_B', 'f_3' })
constraint('v', { 'v_B4', 'v_B' }, { 'v_B', 'f_4' })
constraint('v', { 'v_B5', 'v_B' }, { 'v_B', 'f_5' })

constraint('v', { 'v_J0', 'v_J' }, { 'v_J', 'f_0' })
constraint('v', { 'v_J1', 'v_J' }, { 'v_J', 'f_1' })
constraint('v', { 'v_J2', 'v_J' }, { 'v_J', 'f_2' })
constraint('v', { 'v_J3', 'v_J' }, { 'v_J', 'f_3' })
constraint('v', { 'v_J4', 'v_J' }, { 'v_J', 'f_4' })
constraint('v', { 'v_J5', 'v_J' }, { 'v_J', 'f_5' })


--adjacences des subdivision du volume
--adjacences
--le long des aretes du tetraedre

-- celles-la ok
constraint('v', { 'v_R0', 'v_B' }, { 'v_R5', 'v_V', 'f_inv' })
constraint('v', { 'v_R0', 'v_V' }, { 'v_R1', 'v_J', 'f_rot', 'f_rot', 'f_inv' })
constraint('v', { 'v_R1', 'v_B', 'f_inv' }, { 'v_R2', 'v_J', 'f_rot' })
constraint('v', { 'v_R2', 'v_B', 'f_inv' }, { 'v_R3', 'v_V' })
constraint('v', { 'v_R3', 'v_J', 'f_inv' }, { 'v_R4', 'v_V', 'f_rot', 'f_rot' })
constraint('v', { 'v_R4', 'v_J', 'f_inv' }, { 'v_R5', 'v_B', 'f_rot' })


-- fave verte : celle-la ok
constraint('v', { 'v_V0', 'v_R' }, { 'v_V5', 'v_B', 'f_inv' })
constraint('v', { 'v_V0', 'v_B', 'f_inv' }, { 'v_V1', 'v_J', 'f_rot' })
constraint('v', { 'v_V1', 'v_R', 'f_inv' }, { 'v_V2', 'v_J' })
constraint('v', { 'v_V2', 'v_R', 'f_inv' }, { 'v_V3', 'v_B' })
constraint('v', { 'v_V3', 'v_J', 'f_inv' }, { 'v_V4', 'v_B', 'f_rot' })
constraint('v', { 'v_V4', 'v_J', 'f_inv' }, { 'v_V5', 'v_R' })


-- fave blue : celle-la ok
constraint('v', { 'v_B0', 'v_V', 'f_inv' }, { 'v_B5', 'v_R' })
constraint('v', { 'v_B0', 'v_R', 'f_inv' }, { 'v_B1', 'v_J' })
constraint('v', { 'v_B1', 'v_V', 'f_inv' }, { 'v_B2', 'v_J', 'f_rot', 'f_rot' })
constraint('v', { 'v_B2', 'v_V', 'f_inv' }, { 'v_B3', 'v_R' })
constraint('v', { 'v_B3', 'v_J', 'f_inv' }, { 'v_B4', 'v_R' })
constraint('v', { 'v_B4', 'v_J', 'f_inv' }, { 'v_B5', 'v_V', 'f_rot', 'f_rot' })

-- face jaune : celle-la ok
constraint('v', { 'v_J0', 'v_V' }, { 'v_J5', 'v_B', 'f_inv' })
constraint('v', { 'v_J0', 'v_B' }, { 'v_J1', 'v_R', 'f_inv' })
constraint('v', { 'v_J1', 'v_V' }, { 'v_J2', 'v_R', 'f_inv' })
constraint('v', { 'v_J2', 'v_V' }, { 'v_J3', 'v_B', 'f_inv' })
constraint('v', { 'v_J3', 'v_R' }, { 'v_J4', 'v_B', 'f_inv' })
constraint('v', { 'v_J4', 'v_R' }, { 'v_J5', 'v_V', 'f_inv' })

--         -- a l'interieur "des faces"
constraint('v', { 'v_R1', 'v_V' }, { 'v_V4', 'v_R', 'f_inv' })
constraint('v', { 'v_R2', 'v_V' }, { 'v_V3', 'v_R', 'f_inv' })

constraint('v', { 'v_V1', 'v_B' }, { 'v_B4', 'v_V', 'f_inv' })
constraint('v', { 'v_V2', 'v_B' }, { 'v_B3', 'v_V', 'f_inv' })

constraint('v', { 'v_B1', 'v_R' }, { 'v_R4', 'v_B', 'f_inv' })
constraint('v', { 'v_B2', 'v_R' }, { 'v_R3', 'v_B', 'f_inv' })

constraint('v', { 'v_J0', 'v_R' }, { 'v_R5', 'v_J', 'f_inv' })
constraint('v', { 'v_J5', 'v_R' }, { 'v_R0', 'v_J', 'f_inv' })

constraint('v', { 'v_J1', 'v_B', 'f_inv' }, { 'v_B0', 'v_J', 'f_rot' })
constraint('v', { 'v_J2', 'v_B', 'f_inv' }, { 'v_B5', 'v_J', 'f_rot' })

constraint('v', { 'v_J3', 'v_V', 'f_inv' }, { 'v_V0', 'v_J', 'f_rot', 'f_rot' })
constraint('v', { 'v_J4', 'v_V', 'f_inv' }, { 'v_V5', 'v_J', 'f_rot', 'f_rot' })

--adjacences
raideur1=4

grid("v", {
    -- tous les coins par rapport a la face jaune
    { { "v_R", "intern_0" }, { "v_R", "intern_1" } },
    { { "v_R", "intern_1" }, { "v_R", "intern_2" } },
    { { "v_R", "intern_2" }, { "v_R", "intern_3" } },
    { { "v_R", "intern_3" }, { "v_R", "intern_4" } },
    { { "v_R", "intern_4" }, { "v_R", "intern_5" } },
    { { "v_R", "intern_5" }, { "v_R", "intern_0" } },
    
    { { "v_V", "intern_0" }, { "v_V", "intern_1" } },
    { { "v_V", "intern_1" }, { "v_V", "intern_2" } },
    { { "v_V", "intern_2" }, { "v_V", "intern_3" } },
    { { "v_V", "intern_3" }, { "v_V", "intern_4" } },
    { { "v_V", "intern_4" }, { "v_V", "intern_5" } },
    { { "v_V", "intern_5" }, { "v_V", "intern_0" } },

    { { "v_B", "intern_0" }, { "v_B", "intern_1" } },
    { { "v_B", "intern_1" }, { "v_B", "intern_2" } },
    { { "v_B", "intern_2" }, { "v_B", "intern_3" } },
    { { "v_B", "intern_3" }, { "v_B", "intern_4" } },
    { { "v_B", "intern_4" }, { "v_B", "intern_5" } },
    { { "v_B", "intern_5" }, { "v_B", "intern_0" } },

    { { "v_J", "intern_0" }, { "v_J", "intern_1" } },
    { { "v_J", "intern_1" }, { "v_J", "intern_2" } },
    { { "v_J", "intern_2" }, { "v_J", "intern_3" } },
    { { "v_J", "intern_3" }, { "v_J", "intern_4" } },
    { { "v_J", "intern_4" }, { "v_J", "intern_5" } },
    { { "v_J", "intern_5" }, { "v_J", "intern_0" } },
    -- connexion des faces entre elles
    { { "v_R", "intern_1" }, { "v_V", "intern_4" }, k=0.002 },
    { { "v_R", "intern_2" }, { "v_V", "intern_3" }, k=0.002 },
    { { "v_V", "intern_1" }, { "v_B", "intern_4" }, k=0.002 },
    { { "v_V", "intern_2" }, { "v_B", "intern_3" }, k=0.002 },
    { { "v_B", "intern_1" }, { "v_R", "intern_4" }, k=0.002 },
    { { "v_B", "intern_2" }, { "v_R", "intern_3" }, k=0.002 },
    { { "v_J", "intern_5" }, { "v_R", "intern_0" }, k=0.002 },
    { { "v_J", "intern_0" }, { "v_R", "intern_5" }, k=0.002 },
    { { "v_J", "intern_1" }, { "v_B", "intern_0" }, k=0.002 },
    { { "v_J", "intern_2" }, { "v_B", "intern_5" }, k=0.002 },
    { { "v_J", "intern_3" }, { "v_V", "intern_0" }, k=0.002 },
    { { "v_J", "intern_4" }, { "v_V", "intern_5" }, k=0.002 }
})

primitive("v", {
    --les faces
    {
        { 'v_J', 'intern_0' },
        { 'v_J', 'intern_1' },
        { 'v_J', 'intern_2' },
        { 'v_J', 'intern_3' },
        { 'v_J', 'intern_4' },
        { 'v_J', 'intern_5' },
    },
    {
        { 'v_R', 'intern_0' },
        { 'v_R', 'intern_1' },
        { 'v_R', 'intern_2' },
        { 'v_R', 'intern_3' },
        { 'v_R', 'intern_4' },
        { 'v_R', 'intern_5' },
    },
    {
        { 'v_V', 'intern_0' },
        { 'v_V', 'intern_1' },
        { 'v_V', 'intern_2' },
        { 'v_V', 'intern_3' },
        { 'v_V', 'intern_4' },
        { 'v_V', 'intern_5' },
    },
    {
        { 'v_B', 'intern_0' },
        { 'v_B', 'intern_1' },
        { 'v_B', 'intern_2' },
        { 'v_B', 'intern_3' },
        { 'v_B', 'intern_4' },
        { 'v_B', 'intern_5' },
    },
    -- les aretes
    {
        { 'v_J', 'intern_5' },
        { 'v_R', 'intern_0' },
        { 'v_R', 'intern_5' },
        { 'v_J', 'intern_0' },
    },
    {
        { 'v_J', 'intern_1' },
        { 'v_B', 'intern_0' },
        { 'v_B', 'intern_5' },
        { 'v_J', 'intern_2' },
    },
    {
        { 'v_J', 'intern_3' },
        { 'v_V', 'intern_0' },
        { 'v_V', 'intern_5' },
        { 'v_J', 'intern_4' },
    },
    {
        { 'v_R', 'intern_1' },
        { 'v_V', 'intern_4' },
        { 'v_V', 'intern_3' },
        { 'v_R', 'intern_2' },
    },
    {
        { 'v_V', 'intern_1' },
        { 'v_B', 'intern_4' },
        { 'v_B', 'intern_3' },
        { 'v_V', 'intern_2' },
    },
    {
        { 'v_B', 'intern_1' },
        { 'v_R', 'intern_4' },
        { 'v_R', 'intern_3' },
        { 'v_B', 'intern_2' },
    },
    -- pour les sommets
    {
        { 'v_R', 'intern_0' },
        { 'v_J', 'intern_5' },
        { 'v_J', 'intern_4' },
        { 'v_V', 'intern_5' },
        { 'v_V', 'intern_4' },
        { 'v_R', 'intern_1' },
    },
    {
        { 'v_V', 'intern_0' },
        { 'v_J', 'intern_3' },
        { 'v_J', 'intern_2' },
        { 'v_B', 'intern_5' },
        { 'v_B', 'intern_4' },
        { 'v_V', 'intern_1' },
    },
    {
        { 'v_B', 'intern_0' },
        { 'v_J', 'intern_1' },
        { 'v_J', 'intern_0' },
        { 'v_R', 'intern_5' },
        { 'v_R', 'intern_4' },
        { 'v_B', 'intern_1' },
    },
    {
        { 'v_R', 'intern_2' },
        { 'v_V', 'intern_3' },
        { 'v_V', 'intern_2' },
        { 'v_B', 'intern_3' },
        { 'v_B', 'intern_2' },
        { 'v_R', 'intern_3' },
    },
})

-------------------- geometrie

-- initialisation des points de controle
local d=0.7
local e = 1-d

initMat("f", { "f_0" }, {
    { 1 ,  d ,  d ,  d ,  d ,  d },
    { 0 ,  e ,  0 ,  0 ,  0 ,  0 },
    { 0 ,  0 ,  e ,  0 ,  0 ,  0 },
    { 0 ,  0 ,  0 ,  e ,  0 ,  0 },
    { 0 ,  0 ,  0 ,  0 ,  e ,  0 },
    { 0 ,  0 ,  0 ,  0 ,  0 ,  e }
}, "CONST")

-- calcul position des points de ctrl
-- repartis en hexagone sur chaque face du triangle avec un rapprochement d'un facteur de "scale" du centre de la face
local center = {0,0,0}
local edgeLength = 2
local H=math.sqrt(2./3.)*edgeLength
local h2 = edgeLength/math.sqrt(3.)
local h1 = h2/2.
local ZPlan = -H/4.  -- position z de la base de la pyramide
-- pyramide definie par la base A, B, C et sommet D
local D = {0,0,3*H/4.}
local A = {h2,0,ZPlan}
local x = math.cos(2)
local B = {h2*math.cos(2*math.pi/3.),h2*math.sin(2*math.pi/3.),ZPlan}
local C = {h2*math.cos(4*math.pi/3.),h2*math.sin(4*math.pi/3.),ZPlan}
local BaryABC={(A[1]+B[1]+C[1])/3.,(A[2]+B[2]+C[2])/3.,(A[3]+B[3]+C[3])/3.}
local BaryABD={(A[1]+B[1]+D[1])/3.,(A[2]+B[2]+D[2])/3.,(A[3]+B[3]+D[3])/3.}
local BaryBCD={(C[1]+B[1]+D[1])/3.,(C[2]+B[2]+D[2])/3.,(C[3]+B[3]+D[3])/3.}
local BaryCAD={(A[1]+C[1]+D[1])/3.,(A[2]+C[2]+D[2])/3.,(A[3]+C[3]+D[3])/3.}

-- scale 
local scale = .6  -- facteur d'echelle pour reduire la taille de la face hexagonale => augmente le lacune d'arete

local Bary2AB={
    (2*A[1]+B[1])/3.,
    (2*A[2]+B[2])/3.,
    (2*A[3]+B[3])/3.}
local BaryA2B={
    (A[1]+2*B[1])/3.,
    (A[2]+2*B[2])/3.,
    (A[3]+2*B[3])/3.}
local Bary2BC={
    (2*B[1]+C[1])/3.,
    (2*B[2]+C[2])/3.,
    (2*B[3]+C[3])/3.}
local BaryB2C={
    (B[1]+2*C[1])/3.,
    (B[2]+2*C[2])/3.,
    (B[3]+2*C[3])/3.}
local Bary2CA={
    (2*C[1]+A[1])/3.,
    (2*C[2]+A[2])/3.,
    (2*C[3]+A[3])/3.}
local BaryC2A={
    (C[1]+2*A[1])/3.,
    (C[2]+2*A[2])/3.,
    (C[3]+2*A[3])/3.}


local Bary2BD={
    (2*B[1]+D[1])/3.,
    (2*B[2]+D[2])/3.,
    (2*B[3]+D[3])/3.}
local BaryB2D={
    (B[1]+2*D[1])/3.,
    (B[2]+2*D[2])/3.,
    (B[3]+2*D[3])/3.}
local Bary2CD={
    (2*C[1]+D[1])/3.,
    (2*C[2]+D[2])/3.,
    (2*C[3]+D[3])/3.}
local BaryC2D={
    (C[1]+2*D[1])/3.,
    (C[2]+2*D[2])/3.,
    (C[3]+2*D[3])/3.}

local Bary2AD={
    (2*A[1]+D[1])/3.,
    (2*A[2]+D[2])/3.,
    (2*A[3]+D[3])/3.}
local BaryA2D={
    (A[1]+2*D[1])/3.,
    (A[2]+2*D[2])/3.,
    (A[3]+2*D[3])/3.}

local NormalFaceRouge= {
    BaryABD[1] - center[1],
    BaryABD[2] - center[2],
    BaryABD[3] - center[3]}
local NormalFaceVerte= {
    BaryBCD[1] - center[1],
    BaryBCD[2] - center[2],
    BaryBCD[3] - center[3]}
local NormalFaceBlue = {
    BaryCAD[1] - center[1],
    BaryCAD[2] - center[2],
    BaryCAD[3] - center[3]}

local PtRx={}
local PtRy={}
local PtRz={}

table.insert(PtRx, (BaryA2B[1]-BaryABD[1])*scale+BaryABD[1])
table.insert(PtRy, (BaryA2B[2]-BaryABD[2])*scale+BaryABD[2])
table.insert(PtRz, (BaryA2B[3]-BaryABD[3])*scale+BaryABD[3])

table.insert(PtRx, (Bary2BD[1]-BaryABD[1])*scale+BaryABD[1])
table.insert(PtRy, (Bary2BD[2]-BaryABD[2])*scale+BaryABD[2])
table.insert(PtRz, (Bary2BD[3]-BaryABD[3])*scale+BaryABD[3])

table.insert(PtRx, (BaryB2D[1]-BaryABD[1])*scale+BaryABD[1])
table.insert(PtRy, (BaryB2D[2]-BaryABD[2])*scale+BaryABD[2])
table.insert(PtRz, (BaryB2D[3]-BaryABD[3])*scale+BaryABD[3])

table.insert(PtRx, (BaryA2D[1]-BaryABD[1])*scale+BaryABD[1])
table.insert(PtRy, (BaryA2D[2]-BaryABD[2])*scale+BaryABD[2])
table.insert(PtRz, (BaryA2D[3]-BaryABD[3])*scale+BaryABD[3])

table.insert(PtRx, (Bary2AD[1]-BaryABD[1])*scale+BaryABD[1])
table.insert(PtRy, (Bary2AD[2]-BaryABD[2])*scale+BaryABD[2])
table.insert(PtRz, (Bary2AD[3]-BaryABD[3])*scale+BaryABD[3])


table.insert(PtRx, (Bary2AB[1]-BaryABD[1])*scale+BaryABD[1])
table.insert(PtRy, (Bary2AB[2]-BaryABD[2])*scale+BaryABD[2])
table.insert(PtRz, (Bary2AB[3]-BaryABD[3])*scale+BaryABD[3])

local PtVx={}
local PtVy={}
local PtVz={}

table.insert(PtVx, (BaryB2C[1]-BaryBCD[1])*scale+BaryBCD[1])
table.insert(PtVy, (BaryB2C[2]-BaryBCD[2])*scale+BaryBCD[2])
table.insert(PtVz, (BaryB2C[3]-BaryBCD[3])*scale+BaryBCD[3])

table.insert(PtVx, (Bary2CD[1]-BaryBCD[1])*scale+BaryBCD[1])
table.insert(PtVy, (Bary2CD[2]-BaryBCD[2])*scale+BaryBCD[2])
table.insert(PtVz, (Bary2CD[3]-BaryBCD[3])*scale+BaryBCD[3])

table.insert(PtVx, (BaryC2D[1]-BaryBCD[1])*scale+BaryBCD[1])
table.insert(PtVy, (BaryC2D[2]-BaryBCD[2])*scale+BaryBCD[2])
table.insert(PtVz, (BaryC2D[3]-BaryBCD[3])*scale+BaryBCD[3])

table.insert(PtVx, (BaryB2D[1]-BaryBCD[1])*scale+BaryBCD[1])
table.insert(PtVy, (BaryB2D[2]-BaryBCD[2])*scale+BaryBCD[2])
table.insert(PtVz, (BaryB2D[3]-BaryBCD[3])*scale+BaryBCD[3])

table.insert(PtVx, (Bary2BD[1]-BaryBCD[1])*scale+BaryBCD[1])
table.insert(PtVy, (Bary2BD[2]-BaryBCD[2])*scale+BaryBCD[2])
table.insert(PtVz, (Bary2BD[3]-BaryBCD[3])*scale+BaryBCD[3])

table.insert(PtVx, (Bary2BC[1]-BaryBCD[1])*scale+BaryBCD[1])
table.insert(PtVy, (Bary2BC[2]-BaryBCD[2])*scale+BaryBCD[2])
table.insert(PtVz, (Bary2BC[3]-BaryBCD[3])*scale+BaryBCD[3])

local PtBx={}
local PtBy={}
local PtBz={}

table.insert(PtBx, (BaryC2A[1]-BaryCAD[1])*scale+BaryCAD[1])
table.insert(PtBy, (BaryC2A[2]-BaryCAD[2])*scale+BaryCAD[2])
table.insert(PtBz, (BaryC2A[3]-BaryCAD[3])*scale+BaryCAD[3])

table.insert(PtBx, (Bary2AD[1]-BaryCAD[1])*scale+BaryCAD[1])
table.insert(PtBy, (Bary2AD[2]-BaryCAD[2])*scale+BaryCAD[2])
table.insert(PtBz, (Bary2AD[3]-BaryCAD[3])*scale+BaryCAD[3])

table.insert(PtBx, (BaryA2D[1]-BaryCAD[1])*scale+BaryCAD[1])
table.insert(PtBy, (BaryA2D[2]-BaryCAD[2])*scale+BaryCAD[2])
table.insert(PtBz, (BaryA2D[3]-BaryCAD[3])*scale+BaryCAD[3])

table.insert(PtBx, (BaryC2D[1]-BaryCAD[1])*scale+BaryCAD[1])
table.insert(PtBy, (BaryC2D[2]-BaryCAD[2])*scale+BaryCAD[2])
table.insert(PtBz, (BaryC2D[3]-BaryCAD[3])*scale+BaryCAD[3])

table.insert(PtBx, (Bary2CD[1]-BaryCAD[1])*scale+BaryCAD[1])
table.insert(PtBy, (Bary2CD[2]-BaryCAD[2])*scale+BaryCAD[2])
table.insert(PtBz, (Bary2CD[3]-BaryCAD[3])*scale+BaryCAD[3])

table.insert(PtBx, (Bary2CA[1]-BaryCAD[1])*scale+BaryCAD[1])
table.insert(PtBy, (Bary2CA[2]-BaryCAD[2])*scale+BaryCAD[2])
table.insert(PtBz, (Bary2CA[3]-BaryCAD[3])*scale+BaryCAD[3])


local PtJx={}
local PtJy={}
local PtJz={}

table.insert(PtJx, (Bary2AB[1]-BaryABC[1])*scale+BaryABC[1])
table.insert(PtJy, (Bary2AB[2]-BaryABC[2])*scale+BaryABC[2])
table.insert(PtJz, (Bary2AB[3]-BaryABC[3])*scale+BaryABC[3])

table.insert(PtJx, (BaryC2A[1]-BaryABC[1])*scale+BaryABC[1])
table.insert(PtJy, (BaryC2A[2]-BaryABC[2])*scale+BaryABC[2])
table.insert(PtJz, (BaryC2A[3]-BaryABC[3])*scale+BaryABC[3])

table.insert(PtJx, (Bary2CA[1]-BaryABC[1])*scale+BaryABC[1])
table.insert(PtJy, (Bary2CA[2]-BaryABC[2])*scale+BaryABC[2])
table.insert(PtJz, (Bary2CA[3]-BaryABC[3])*scale+BaryABC[3])

table.insert(PtJx, (BaryB2C[1]-BaryABC[1])*scale+BaryABC[1])
table.insert(PtJy, (BaryB2C[2]-BaryABC[2])*scale+BaryABC[2])
table.insert(PtJz, (BaryB2C[3]-BaryABC[3])*scale+BaryABC[3])

table.insert(PtJx, (Bary2BC[1]-BaryABC[1])*scale+BaryABC[1])
table.insert(PtJy, (Bary2BC[2]-BaryABC[2])*scale+BaryABC[2])
table.insert(PtJz, (Bary2BC[3]-BaryABC[3])*scale+BaryABC[3])

table.insert(PtJx, (BaryA2B[1]-BaryABC[1])*scale+BaryABC[1])
table.insert(PtJy, (BaryA2B[2]-BaryABC[2])*scale+BaryABC[2])
table.insert(PtJz, (BaryA2B[3]-BaryABC[3])*scale+BaryABC[3])


local function concatTables(...)
    local result = {}
    local index = 1

    for _, t in ipairs({...}) do
        for i = 1, #t do
            result[index] = t[i]
            index = index + 1
        end
    end

    return result
end

local Ptx = concatTables(PtRx, PtVx, PtBx, PtJx)
local Pty = concatTables(PtRy, PtVy, PtBy, PtJy)
local Ptz = concatTables(PtRz, PtVz, PtBz, PtJz)

initMat("init", { "init_0" }, {
    Ptx,
    Pty,
    Ptz
}, "VAR")