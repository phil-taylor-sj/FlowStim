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
            mesh = std::move(factory(vecp::Vec2i(3, 4), vecp::Vec2d(3., 8.)));
        }
        std::unique_ptr<const Mesh> mesh;
    };

    class SmallMeshEqualSetsF : public ::testing::Test
    {
    protected:
        void SetUp() override 
        {
            MeshFactory factory = MeshFactory();
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
        std::unique_ptr<Mesh> mesh;
    };

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

    TEST_F(SmallMeshEqualF, test_sets_correct_face_positions)
    {
        double expected[31][2] = {
                {0., 1.}, {1., 1.}, {2., 1.}, {3., 1.},
                {0., 3.}, {1., 3.}, {2., 3.}, {3., 3.},
                {0., 5.}, {1., 5.}, {2., 5.}, {3., 5.},
                {0., 7.}, {1., 7.}, {2., 7.}, {3., 7.},
                {0.5, 0.}, {1.5, 0.}, {2.5, 0.},
                {0.5, 2.}, {1.5, 2.}, {2.5, 2.},
                {0.5, 4.}, {1.5, 4.}, {2.5, 4.}, 
                {0.5, 6.}, {1.5, 6.}, {2.5, 6.}, 
                {0.5, 8.}, {1.5, 8.}, {2.5, 8.}
        };
        for (int id = 0; id < 31; id++)
        {
            ASSERT_DOUBLE_EQ(expected[id][0], mesh->faces[id].center.x);
            ASSERT_DOUBLE_EQ(expected[id][1], mesh->faces[id].center.y);
        }
    }

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
            const Face& face = mesh->faces[id];
            Vec2d centerOne = mesh->cells[face.ownerId].center;
            Vec2d centerTwo = (face.neighId != -1)
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
}