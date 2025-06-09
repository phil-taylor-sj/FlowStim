#include "../pch.h"

#include <set>

#include <Core/Domain/Mesh2dStructuredFactory.h>

using namespace fstim;

namespace Domain_Tests
{
    class SmallMeshEqualF : public ::testing::Test
    {
    protected:
        void SetUp() override 
        {
            Mesh2dStructuredFactory factory = Mesh2dStructuredFactory();
            mesh = std::move(factory(vecp::Vec2i(3, 4), vecp::Vec2d(3., 8.)));
        }
        std::unique_ptr<const Mesh2d> mesh;
    };

    class SmallMeshEqualParamF : public ::testing::TestWithParam<std::tuple<vecp::Vec2i, vecp::Vec2d>>
    {
    protected:
        void SetUp() override 
        {
            size = std::get<0>(GetParam());
            length = std::get<1>(GetParam());
            dx = length.x / size.x;
            dy = length.y / size.y;
            Mesh2dStructuredFactory factory = Mesh2dStructuredFactory();
            mesh = std::move(factory(size, length));
        }
        vecp::Vec2i size;
        vecp::Vec2d length;
        std::unique_ptr<const Mesh2d> mesh;
        double dx, dy;
    };

    class SmallMeshEqualSetsF : public ::testing::Test
    {
    protected:
        void SetUp() override 
        {
            Mesh2dStructuredFactory factory = Mesh2dStructuredFactory();
            mesh = std::move(factory(vecp::Vec2i(3, 4), vecp::Vec2d(3., 8.)));
            mesh->addFaceSet(vecp::Vec2d(1.5, 8.), vecp::Vec2d(3., 0.001));
            
            std::vector<std::tuple<vecp::Vec2d, vecp::Vec2d>> walls = 
            {
                std::make_tuple<>(vecp::Vec2d(0., 4.), vecp::Vec2d(0.001, 8.)),
                std::make_tuple<>(vecp::Vec2d(3., 4.), vecp::Vec2d(0.001, 8.)),
                std::make_tuple<>(vecp::Vec2d(1.5, 0.), vecp::Vec2d(3., 0.001))
            };
            
            mesh->addFaceSet(walls); 
        }
        std::unique_ptr<Mesh2d> mesh;
    };

    class SmallMeshEqualCellCenter_F : public SmallMeshEqualParamF {};
    TEST_P(SmallMeshEqualCellCenter_F, CorrectCellCenterPositions)
    {
        double expected[12][2]  = {
            {0.5, 0.5}, {1.5, 0.5}, {2.5, 0.5}, 
            {0.5, 1.5}, {1.5, 1.5}, {2.5, 1.5},
            {0.5, 2.5}, {1.5, 2.5}, {2.5, 2.5},
            {0.5, 3.5}, {1.5, 3.5}, {2.5, 3.5}
        };
        for (int id = 0; id < 12; id++)
        {
            ASSERT_DOUBLE_EQ(expected[id][0] * dx, mesh->cells[id].center.x);
            ASSERT_DOUBLE_EQ(expected[id][1] * dy, mesh->cells[id].center.y);
        }
    }

    INSTANTIATE_TEST_SUITE_P(CorrectCellCenterPositions, SmallMeshEqualCellCenter_F, testing::Values(
        std::make_tuple(vecp::Vec2i(3, 4), vecp::Vec2d(3., 8.)),
        std::make_tuple(vecp::Vec2i(3, 4), vecp::Vec2d(1., 1.)),
        std::make_tuple(vecp::Vec2i(3, 4), vecp::Vec2d(0.01, 0.01))
    ));

    TEST_F(SmallMeshEqualF, test_sets_correct_face_ids_for_cells)
    {
        int expectedSouth[12] = {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27};
        int expectedNorth[12] = {19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30};
        int expectedEast[12] = {1, 2, 3, 5, 6, 7, 9, 10, 11, 13, 14, 15};
        int expectedWest[12] = {0, 1, 2, 4, 5, 6, 8, 9, 10, 12, 13, 14};

        for (int id = 0; id < 12; id++)
        {
            ASSERT_EQ(expectedNorth[id], mesh->cells[id].faceId[(int)Compass::NORTH]);
            ASSERT_EQ(expectedSouth[id], mesh->cells[id].faceId[(int)Compass::SOUTH]);
            ASSERT_EQ(expectedEast[id], mesh->cells[id].faceId[(int)Compass::EAST]);
            ASSERT_EQ(expectedWest[id], mesh->cells[id].faceId[(int)Compass::WEST]);
        }
    }

    TEST_F(SmallMeshEqualF, test_sets_correct_owner_ids_for_faces)
    {   
        int expected[] = {0, 0, 1, 2, 3, 3, 4, 5, 6, 6, 7, 8, 9, 9, 10 ,11,
                    0, 1, 2, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
        for (int id = 0; id < 31; id++)
        {
            ASSERT_EQ(expected[id], mesh->faces[id].ownerId);
        }       
    }

    TEST_F(SmallMeshEqualF, test_sets_correct_neighbour_ids_for_faces)
    {   
        int expected[] = {-1, 1, 2, -1, -1, 4, 5, -1, -1, 7, 8, -1, -1, 10, 11, -1,
            -1, -1, -1, 3, 4, 5, 6, 7, 8, 9, 10, 11, -1, -1, -1};
        for (int id = 0; id < 31; id++)
        {
            ASSERT_EQ(expected[id], mesh->faces[id].neighId);
        }       
    }

    class SmallMeshEqualFaceCenter_F : public SmallMeshEqualParamF {};
    TEST_P(SmallMeshEqualFaceCenter_F, CorrectFacePositions)
    {
        double expected[31][2] = {
                {0., 0.5}, {1., 0.5}, {2., 0.5}, {3., 0.5},
                {0., 1.5}, {1., 1.5}, {2., 1.5}, {3., 1.5},
                {0., 2.5}, {1., 2.5}, {2., 2.5}, {3., 2.5},
                {0., 3.5}, {1., 3.5}, {2., 3.5}, {3., 3.5},
                {0.5, 0.}, {1.5, 0.}, {2.5, 0.},
                {0.5, 1.}, {1.5, 1.}, {2.5, 1.},
                {0.5, 2.}, {1.5, 2.}, {2.5, 2.}, 
                {0.5, 3.}, {1.5, 3.}, {2.5, 3.}, 
                {0.5, 4.}, {1.5, 4.}, {2.5, 4.}
        };
        for (int id = 0; id < 31; id++)
        {
            ASSERT_DOUBLE_EQ(expected[id][0] * dx, mesh->faces[id].center.x);
            ASSERT_DOUBLE_EQ(expected[id][1] * dy, mesh->faces[id].center.y);
        }
    }

    INSTANTIATE_TEST_SUITE_P(CorrectFacePositions, SmallMeshEqualFaceCenter_F, testing::Values(
        std::make_tuple(vecp::Vec2i(3, 4), vecp::Vec2d(3., 8.)),
        std::make_tuple(vecp::Vec2i(3, 4), vecp::Vec2d(200., 100.)),
        std::make_tuple(vecp::Vec2i(3, 4), vecp::Vec2d(1., 1.))
    ));

    TEST_F(SmallMeshEqualF, test_sets_correct_face_normal_vectors)
    {
        double dx = 1.;
        double dy = 2.;
        double expected[31][2] = {
            {-dy, 0,}, {dy, 0.}, {dy, 0,}, {dy, 0.},
            {-dy, 0,}, {dy, 0.}, {dy, 0,}, {dy, 0.}, 
            {-dy, 0,}, {dy, 0.}, {dy, 0,}, {dy, 0.}, 
            {-dy, 0,}, {dy, 0.}, {dy, 0,}, {dy, 0.},
            {0., -dx}, {0., -dx}, {0., -dx},
            {0., dx}, {0., dx}, {0., dx},
            {0., dx}, {0., dx}, {0., dx},
            {0., dx}, {0., dx}, {0., dx},
            {0., dx}, {0., dx}, {0., dx}
        };
        for (int id = 0; id < 31; id++)
        {
            ASSERT_DOUBLE_EQ(expected[id][0], mesh->faces[id].normal.x);
            ASSERT_DOUBLE_EQ(expected[id][1], mesh->faces[id].normal.y);
        }
    }

    TEST_F(SmallMeshEqualF, test_sets_correct_cell_spacings_for_faces)
    {
        double expected[31] = {
            0.5, 1., 1., 0.5, 0.5, 1., 1., 0.5,
            0.5, 1., 1., 0.5, 0.5, 1., 1., 0.5,
            1., 1., 1.,
            2., 2., 2.,
            2., 2., 2.,
            2., 2., 2.,
            1., 1., 1.
        };
        for (int id = 0; id < 31; id++)
        {
            const Face2d& face = mesh->faces[id];
            vecp::Vec2d centerOne = mesh->cells[face.ownerId].center;
            vecp::Vec2d centerTwo = (face.neighId != -1)
                ? mesh->cells[face.neighId].center
                : face.center;
            double spacing = centerOne.mag(centerTwo);
            ASSERT_NEAR(expected[id], spacing, 1E-06);
        }
    }

    TEST_F(SmallMeshEqualF, test_sets_correct_cell_volumes)
    {
        for (int id = 0; id < 12; id++)
        {
             ASSERT_DOUBLE_EQ(2., mesh->cells[id].volume);
        }
    }

    TEST_F(SmallMeshEqualF, test_sets_correct_domain_properties)
    {
        ASSERT_EQ(12, mesh->nCells);
        ASSERT_EQ(31, mesh->nFaces);
    }

    TEST_F(SmallMeshEqualSetsF, test_assigns_boundary_cells_to_correct_face_sets)
    {
        std::set<int> set_1{28, 29, 30};
        std::set<int> set_2{0, 3, 4, 7, 8, 11, 12, 15, 16, 17, 18};
        for (int faceId = 0; faceId < mesh->nFaces; faceId++)
        {
            int setId = mesh->getFaceSetId(faceId);
            int expected = (set_1.count(faceId) == 1)
                ? 0
                : (set_2.count(faceId) == 1) 
                    ? 1 
                    : -1;
            ASSERT_EQ(expected, setId);
        }
    }

    class Mesh_Vertex_Fixture : public testing::TestWithParam<std::tuple<vecp::Vec2i, vecp::Vec2d>>
    {
    protected:
        void SetUp() override
        {
            size = std::get<0>(GetParam());
            length = std::get<1>(GetParam());
            expected = (size.x + 1) * (size.y + 1);
        }
        Mesh2dStructuredFactory factory{};
        vecp::Vec2i size;
        vecp::Vec2d length;
        int expected;
    };

    class MeshVertex_F : public Mesh_Vertex_Fixture {};
    TEST_P(MeshVertex_F, MeshVertex_Valid)
    {
        try
        {
           factory(size, length); 
        }
        catch (const std::exception& err)
        {
            //FAIL() << err.what();
        }

        std::unique_ptr<const Mesh2d> mesh = factory(size, length);
        ASSERT_EQ(mesh->nVertices, expected);

        for (int id = 0; id < mesh->nVertices; id++)
        {
            ASSERT_EQ(mesh->vertices[id].vertexId, id);
        }
    }

    INSTANTIATE_TEST_SUITE_P(MeshVertex_Valid, MeshVertex_F, testing::Values(
        std::make_tuple(vecp::Vec2i(3, 4), vecp::Vec2d(3., 8.)),
        std::make_tuple(vecp::Vec2i(10, 20), vecp::Vec2d(10., 20.)),
        std::make_tuple(vecp::Vec2i(100, 200), vecp::Vec2d(1000., 2000.)),
        std::make_tuple(vecp::Vec2i(3, 4), vecp::Vec2d(0.01, 0.01))
    ));

    TEST_F(SmallMeshEqualF, test_sets_correct_positions_of_vertices)
    {
        double expected[20][2] = {
            {0., 0.}, {0., 2.}, {1., 2.}, {1., 0.}, 
            {2., 2.}, {2., 0.}, {3., 2.}, {3., 0.},
            {0., 4.}, {1., 4.}, {2., 4.}, {3., 4.},
            {0., 6.}, {1., 6.}, {2., 6.}, {3., 6.},
            {0., 8.}, {1., 8.}, {2., 8.}, {3., 8.}
        };

        for (int id = 0; id < 20; id++)
        {
            ASSERT_NEAR(expected[id][0], mesh->vertices[id].position.x, 1E-08);
            ASSERT_NEAR(expected[id][1], mesh->vertices[id].position.y, 1E-08);
        }
    }

    TEST_F(SmallMeshEqualF, test_sets_correct_vertex_ids_to_cells)
    {
        int expected[12][4] = {
            {1, 2, 3, 0},
            {2, 4, 5, 3},
            {4, 6, 7, 5},
            {8, 9, 2, 1},
            {9, 10, 4, 2},
            {10, 11, 6, 4},
            {12, 13, 9, 8},
            {13, 14, 10, 9},
            {14, 15, 11, 10},
            {16, 17, 13, 12},
            {17, 18, 14, 13},
            {18, 19, 15, 14}
        };

        for (int cellId = 0; cellId < mesh->nCells; cellId++)
        {
            for (size_t index = 0; index < 4; index++)
            {
                ASSERT_EQ(expected[cellId][index], mesh->cells[cellId].vertexId[index]);
            }
        }
    }

    TEST_F(SmallMeshEqualF, test_sets_correct_cell_ids_to_vertices)
    {
        std::vector<int> expected[20] = {
            {0}, // 0
            {0, 3}, // 1
            {0, 1, 3, 4}, // 2
            {0, 1}, // 3
            {1, 2, 4, 5}, // 4
            {1, 2}, // 5
            {2, 5}, // 6
            {2}, // 7
            {3, 6}, // 8
            {3, 4, 6, 7}, // 9
            {4, 5, 7, 8}, // 10
            {5, 8}, // 11
            {6, 9}, // 12
            {6, 7, 9, 10}, // 13
            {7, 8, 10, 11}, // 14
            {8, 11}, // 15
            {9}, // 16
            {9, 10}, // 17
            {10, 11}, // 18
            {11} //19
        };

        for (int vertexId = 0; vertexId < mesh->nVertices; vertexId++)
        {
            for (int index = 0; index < expected[vertexId].size(); index++)
            {
                ASSERT_EQ(expected[vertexId][index], mesh->vertices[vertexId].cellId[index]);
            }
        }
    }

    TEST_F(SmallMeshEqualF, test_sets_correct_cell_weights_to_vertices)
    {
        std::vector<double> expected[20] = {
            {1.}, // 0
            {0.5, 0.5}, // 1
            {0.25, 0.25, 0.25, 0.25}, // 2
            {0.5, 0.5}, // 3
            {0.25, 0.25, 0.25, 0.25}, // 4
            {0.5, 0.5}, // 5
            {0.5, 0.5}, // 6
            {1.}, // 7
            {0.5, 0.5}, // 8
            {0.25, 0.25, 0.25, 0.25}, // 9
            {0.25, 0.25, 0.25, 0.25}, // 10
            {0.5, 0.5}, // 11
            {0.5, 0.5}, // 12
            {0.25, 0.25, 0.25, 0.25}, // 13
            {0.25, 0.25, 0.25, 0.25}, // 14
            {0.5, 0.5}, // 15
            {1.}, // 16
            {0.5, 0.5}, // 17
            {0.5, 0.5}, // 18
            {1.} //19
        };

        for (int vertexId = 0; vertexId < mesh->nVertices; vertexId++)
        {
            for (int index = 0; index < expected[vertexId].size(); index++)
            {
               ASSERT_NEAR(expected[vertexId][index], mesh->vertices[vertexId].cellWeight[index], 1E-08);
            }
        }
    }

    class SmallMeshEqualOwnerWeights_F : public SmallMeshEqualParamF {};
    TEST_P(SmallMeshEqualOwnerWeights_F, CorrectOwnerWeights)
    {
        std::set<int> boundaryIds = {0, 3, 4, 7, 8, 11, 12, 15, 16, 17, 18, 28, 29, 30};
        for (int faceId = 0; faceId < 31; faceId++)
        {
            double expected = (boundaryIds.count(faceId) == 1) ? 1. : 0.5;
            ASSERT_NEAR(mesh->faces[faceId].ownerWeight, expected, 1e-06);
        }
    }

    INSTANTIATE_TEST_SUITE_P(CorrectOwnerWeights, SmallMeshEqualOwnerWeights_F, testing::Values(
        std::make_tuple(vecp::Vec2i(3, 4), vecp::Vec2d(3., 8.)),
        std::make_tuple(vecp::Vec2i(3, 4), vecp::Vec2d(200., 100.)),
        std::make_tuple(vecp::Vec2i(3, 4), vecp::Vec2d(1., 1.)),
        std::make_tuple(vecp::Vec2i(3, 4), vecp::Vec2d(0.2, 0.4))
    ));

    class SmallMeshEqualCellToCellSpacing_F : public SmallMeshEqualParamF {};
    TEST_P(SmallMeshEqualCellToCellSpacing_F, CorrectCellToCellSpacing)
    {
        std::set<int> boundaryIds = {0, 3, 4, 7, 8, 11, 12, 15, 16, 17, 18, 28, 29, 30};
        for (int faceId = 0; faceId < 31; faceId++)
        {
            double expected = (faceId < 16) ? this->dx : this->dy;
            expected = (boundaryIds.count(faceId) == 1) ? expected * 0.5 : expected;
            ASSERT_NEAR(mesh->faces[faceId].cellToCellSpacing, expected, 1e-06);
        }
    }

    INSTANTIATE_TEST_SUITE_P(CorrectCellToCellSpacing, SmallMeshEqualCellToCellSpacing_F, testing::Values(
        std::make_tuple(vecp::Vec2i(3, 4), vecp::Vec2d(3., 8.)),
        std::make_tuple(vecp::Vec2i(3, 4), vecp::Vec2d(200., 100.)),
        std::make_tuple(vecp::Vec2i(3, 4), vecp::Vec2d(1., 1.)),
        std::make_tuple(vecp::Vec2i(3, 4), vecp::Vec2d(0.2, 0.4))
    ));
}