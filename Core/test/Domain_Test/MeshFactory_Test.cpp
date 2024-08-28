#include "../pch.h"

#include <Core/Domain/MeshFactory.h>

using namespace fstim;

namespace Domain_Tests
{
    class SmallMeshEqualF : public ::testing::Test
    {
    protected:
        void SetUp() override 
        {
            MeshFactory factory = MeshFactory();
            mesh = std::move(factory(Vec2i(3, 4), Vec2d(3., 8.)));
        }
        std::unique_ptr<const Mesh> mesh;
    };

    // TEST_F(SmallMeshEqualF, test_sets_correct_cell_locations)
    // {
    //     int expectedId[4][3] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {9, 10, 11}};
    //     for (int i = 0; i < 3; i++)
    //     {
    //         for(int j = 0; j < 4; j++)
    //         {
    //             int id = expectedId[i][j];
    //             ASSERT_EQ(i, mesh->cells[id].i);
    //             ASSERT_EQ(j, mesh->cells[id].j);
    //         }
    //     }
    // }

    TEST_F(SmallMeshEqualF, test_sets_correct_cell_center_positions)
    {
        double expected[12][2]  = {
            {0.5, 1.}, {1.5, 1.}, {2.5, 1.}, 
            {0.5, 3.}, {1.5, 3.}, {2.5, 3.},
            {0.5, 5.}, {1.5, 5.}, {2.5, 5.},
            {0.5, 7.}, {1.5, 7.}, {2.5, 7.}
        };
        for (int id = 0; id < 12; id++)
        {
            ASSERT_DOUBLE_EQ(expected[id][0], mesh->cells[id].center.x);
            ASSERT_DOUBLE_EQ(expected[id][1], mesh->cells[id].center.y);
        }
    }

    // TEST_F(SmallMeshEqualF, test_sets_correct_neighbour_cell_ids)
    // {
    //     int expectedNorth[12] = {-1, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8};
    //     int expectedSouth[12] = {3, 4, 5, 6, 7, 8, 9, 10, 11, -1, -1, -1};
    //     int expectedEast[12] = {1, 2, -1, 4, 5, -1, 7, 8, -1, 10, 11, -1};
    //     int expectedWest[12] = {-1, 0, 1, -1, 3, 4, -1, 6, 7, -1, 9, 10};
    //     for (int id = 0; id < 12; id++)
    //     {
    //         ASSERT_EQ(expectedNorth[id], mesh->cells[id].neighId[Compass::NORTH]);
    //         ASSERT_EQ(expectedSouth[id], mesh->cells[id].neighId[Compas::SOUTH]);
    //         ASSERT_EQ(expectedEast[id], mesh->cells[id].neighId[Compass::EAST]);
    //         ASSERT_EQ(expectedWest[id], mesh->cells[id].neighId[Compass::WEST]);
    //     }
    // }

    TEST_F(SmallMeshEqualF, test_sets_correct_face_ids_for_cells)
    {
        int expectedNorth[12] = {16, 21, 26, 17, 22, 27, 18, 23, 28, 19, 24, 29};
        int expectedSouth[12] = {17, 22, 27, 18, 23, 28, 19, 24, 29, 20, 25, 30};
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

    // TEST_F(SmallMeshEqualF, test_sets_correct_cells_ids_for_faces)
    // {   
    //     int expectedNorth[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    //         -1, 0, 3, 6, 9, -1, 1, 4, 7, 10, -1, 2, 5, 8, 11};
    //     int expectedSouth[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    //         0, 3, 6, 9, -1, 1, 4, 7, 10, -1, 2, 5, 8, 11, -1};
    //     int expectedEast[] = {0, 1, 2, -1, 3, 4, 5, -1, 6, 7, 8, -1, 9, 10, 11, -1,
    //         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    //     int expectedWest[] = {-1, 0, 1, 2, -1, 3, 4, 5, -1, 6, 7, 8, -1, 9, 10, 11,
    //         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    //     for (int id = 0; id < 31; id++)
    //     {
    //         ASSERT_EQ(expectedNorth[id], mesh.faces[id].cellId[Compass.NORTH]);
    //         ASSERT_EQ(expectedSouth[id], mesh.faces[id].cellId[Compass.SOUTH]);
    //         ASSERT_EQ(expectedEast[id], mesh.faces[id].cellId[Compass.EAST]);
    //         ASSERT_EQ(expectedWest[id], mesh.faces[id].cellId[Compass.WEST]);
    //     }       
    // }

    TEST_F(SmallMeshEqualF, test_sets_correct_face_positions)
    {
        double expected[31][2] = {
                {0., 1.}, {1., 1.}, {2., 1.}, {3., 1.},
                {0., 3.}, {1., 3.}, {2., 3.}, {3., 3.},
                {0., 5.}, {1., 5.}, {2., 5.}, {3., 5.},
                {0., 7.}, {1., 7.}, {2., 7.}, {3., 7.},
                {0.5, 0.}, {0.5, 2.}, {0.5, 4.}, {0.5, 6.}, {0.5, 8.},
                {1.5, 0.}, {1.5, 2.}, {1.5, 4.}, {1.5, 6.}, {1.5, 8.},
                {2.5, 0.}, {2.5, 2.}, {2.5, 4.}, {2.5, 6.}, {2.5, 8.}
        };
        for (int id = 0; id < 31; id++)
        {
            ASSERT_DOUBLE_EQ(expected[id][0], mesh->faces[id].center.x);
            ASSERT_DOUBLE_EQ(expected[id][1], mesh->faces[id].center.y);
        }
    }

    // TEST_F(SmallMeshEqualF, test_sets_correct_face_area)
    // {
    //     for (int id = 0; id < 31; id++)
    //     {
    //         double expected = id >= 16 ? 1. : 2.;
    //         ASSERT_DOUBLE_EQ(expected, mesh->faces[id].area);
    //     }
    // }

    TEST_F(SmallMeshEqualF, test_sets_correct_cell_spacings_for_faces)
    {
        double expected[31] = {
            0.5, 1., 1., 0.5, 0.5, 1., 1., 0.5,
            0.5, 1., 1., 0.5, 0.5, 1., 1., 0.5,
            1., 2., 2., 2., 1.,
            1., 2., 2., 2., 1.,
            1., 2., 2., 2., 1.
        };
        for (int id = 0; id < 31; id++)
        {
            const Face& face = mesh->faces[id];
            Vec2d centerOne = mesh->cells[face.ownerId].center;
            Vec2d centerTwo = (face.neighId != -1)
                ? mesh->cells[face.neighId].center
                : face.center;
            double spacing = centerOne.mag(centerTwo);
            ASSERT_NEAR(expected[id], spacing, 1E-06);
        }
    }

    // TEST_F(SmallMeshEqualF, test_sets_correct_cell_dimensions)
    // {
    //     for (int id = 0; id < 12; id++)
    //     {
    //         ASSERT_DOUBLE_EQ(1., mesh.cells[id].width.x);
    //         ASSERT_DOUBLE_EQ(2., mesh.cells[id].width.y);
    //     }
    // }

    TEST_F(SmallMeshEqualF, test_sets_correct_domain_properties)
    {
        ASSERT_EQ(12, mesh->nCells);
        ASSERT_EQ(31, mesh->nFaces);
    }
}