#ifndef SHAPE_H
#define SHAPE_H

#include <simple2d.h>
#include "precompiled.h"

#define MaxPolyVertexCount 4

struct Shape
{
    enum Type
    {
        eCircle,
        ePoly,
        eCount
    };
    Body *body;

    // For circle shape
    double radius;

    // For Polygon shape
    Mat2 u; // Orientation matrix from model to world

    Shape() {}
    virtual Shape *Clone(void) const = 0;
    virtual void Initialize(void) = 0;
    virtual void ComputeMass(double density) = 0;
    virtual void SetOrient(double radians) = 0;
    virtual void Draw(void) const = 0;
    virtual Type GetType(void) const = 0;
};

struct Circle : public Shape
{
    Circle(double r)
    {
        radius = r;
    }

    Shape *Clone(void) const
    {
        return new Circle(radius);
    }

    void Initialize(void)
    {
        ComputeMass(1.0f);
    }

    void ComputeMass(double density)
    {
        body->m = PI * radius * radius * density * 0.0001;
        body->im = (body->m) ? 1.0f / body->m : 0.0f;
        body->I = body->m * radius * radius;
        body->iI = (body->I) ? 1.0f / body->I : 0.0f;
    }

    void SetOrient(double radians)
    {
    }

    void Draw(void) const
    {
        S2D_DrawCircle(body->position.x, body->position.y, radius, 100, body->r, body->g, body->b, 1);
    }

    Type GetType(void) const
    {
        return eCircle;
    }
};

struct PolygonShape : public Shape
{
    void Initialize(void)
    {
        ComputeMass(1.0f);
    }

    Shape *Clone(void) const
    {
        PolygonShape *poly = new PolygonShape();
        poly->u = u;
        for (int i = 0; i < m_vertexCount; ++i)
        {
            poly->m_vertices[i] = m_vertices[i];
            poly->m_normals[i] = m_normals[i];
        }
        poly->m_vertexCount = m_vertexCount;
        return poly;
    }

    void ComputeMass(double density)
    {
        // Calculate centroid and moment of interia
        Vec c(0.0f, 0.0f); // centroid
        double area = 0.0f;
        double I = 0.0f;
        const double k_inv3 = 1.0f / 3.0f;

        for (int i1 = 0; i1 < m_vertexCount; ++i1)
        {
            // Triangle vertices, third vertex implied as (0, 0)
            Vec p1(m_vertices[i1]);
            int i2 = i1 + 1 < m_vertexCount ? i1 + 1 : 0;
            Vec p2(m_vertices[i2]);

            double D = Cross(p1, p2);
            double triangleArea = 0.5f * D;

            area += triangleArea;

            // Use area to weight the centroid average, not just vertex position
            c += triangleArea * k_inv3 * (p1 + p2);

            double intx2 = p1.x * p1.x + p2.x * p1.x + p2.x * p2.x;
            double inty2 = p1.y * p1.y + p2.y * p1.y + p2.y * p2.y;
            I += (0.25f * k_inv3 * D) * (intx2 + inty2);
        }

        c *= 1.0f / area;

        // Translate vertices to centroid (make the centroid (0, 0)
        // for the polygon in model space)
        // Not doublely necessary, but I like doing this anyway
        for (int i = 0; i < m_vertexCount; ++i)
            m_vertices[i] -= c;

        body->m = density * area;
        body->im = (body->m) ? 1.0f / body->m : 0.0f;
        body->I = I * density;
        body->iI = body->I ? 1.0f / body->I : 0.0f;
    }

    void SetOrient(double radians)
    {
        u.Set(radians);
    }

    void Draw(void) const
    {
        
        if (m_vertexCount == 3)
        {
            Vec v[m_vertexCount];
            for (int i = 0; i < m_vertexCount; i++)
            {
                v[i] = body->position + u * m_vertices[i];
            }
            S2D_DrawTriangle(
                v[0].x, v[0].y, body->r, body->g, body->b, 1,
                v[1].x, v[1].y, body->r, body->g, body->b, 1,
                v[2].x, v[2].y, body->r, body->g, body->b, 1);
            // std::cout<<"----------------"<<std::endl;
            // std::cout<<v[0].x<<" "<<v[0].y<<std::endl;
            // std::cout<<v[1].x<<" "<<v[1].y<<std::endl;
            // std::cout<<v[2].x<<" "<<v[2].y<<std::endl;
            // std::cout<<"----------------"<<std::endl;

        }
        else if (m_vertexCount == 4)
        {
            Vec v[m_vertexCount];
            for (int i = 0; i < m_vertexCount; i++)
            {
                v[i] = body->position + u * m_vertices[i];
            }
            S2D_DrawQuad(
                v[0].x, v[0].y, body->r, body->g, body->b, 1,
                v[1].x, v[1].y, body->r, body->g, body->b, 1,
                v[2].x, v[2].y, body->r, body->g, body->b, 1,
                v[3].x, v[3].y, body->r, body->g, body->b, 1);

            // std::cout<<"----------------"<<std::endl;
            // std::cout<<v[0].x<<" "<<v[0].y<<std::endl;
            // std::cout<<v[1].x<<" "<<v[1].y<<std::endl;
            // std::cout<<v[2].x<<" "<<v[2].y<<std::endl;
            // std::cout<<v[3].x<<" "<<v[3].y<<std::endl;
            // std::cout<<"----------------"<<std::endl;
        }
    }

    Type GetType(void) const
    {
        return ePoly;
    }

    // Half width and half height
    void SetBox(double hw, double hh)
    {
        m_vertexCount = 4;
        m_vertices[0].Set(-hw, -hh);
        m_vertices[1].Set(hw, -hh);
        m_vertices[2].Set(hw, hh);
        m_vertices[3].Set(-hw, hh);
        m_normals[0].Set(0.0f, -1.0f);
        m_normals[1].Set(1.0f, 0.0f);
        m_normals[2].Set(0.0f, 1.0f);
        m_normals[3].Set(-1.0f, 0.0f);
    }

    void Set(Vec *vertices, int count)
    {
        // No hulls with less than 3 vertices (ensure actual polygon)
        assert(count > 2 && count <= MaxPolyVertexCount);
        count = std::min((int)count, MaxPolyVertexCount);

        // Find the right most point on the hull
        int rightMost = 0;
        double highestXCoord = vertices[0].x;
        for (int i = 1; i < count; ++i)
        {
            double x = vertices[i].x;
            if (x > highestXCoord)
            {
                highestXCoord = x;
                rightMost = i;
            }

            // If matching x then take farthest negative y
            else if (x == highestXCoord)
                if (vertices[i].y < vertices[rightMost].y)
                    rightMost = i;
        }

        int hull[MaxPolyVertexCount];
        int outCount = 0;
        int indexHull = rightMost;

        for (;;)
        {
            hull[outCount] = indexHull;

            // Search for next index that wraps around the hull
            // by computing cross products to find the most counter-clockwise
            // vertex in the set, given the previos hull index
            int nextHullIndex = 0;
            for (int i = 1; i < (int)count; ++i)
            {
                // Skip if same coordinate as we need three unique
                // points in the set to perform a cross product
                if (nextHullIndex == indexHull)
                {
                    nextHullIndex = i;
                    continue;
                }

                // Cross every set of three unique vertices
                // Record each counter clockwise third vertex and add
                // to the output hull
                // See : http://www.oocities.org/pcgpe/math2d.html
                Vec e1 = vertices[nextHullIndex] - vertices[hull[outCount]];
                Vec e2 = vertices[i] - vertices[hull[outCount]];
                double c = Cross(e1, e2);
                if (c < 0.0f)
                    nextHullIndex = i;

                // Cross product is zero then e vectors are on same line
                // therefor want to record vertex farthest along that line
                if (c == 0.0f && e2.squared_vec_length() > e1.squared_vec_length())
                    nextHullIndex = i;
            }

            ++outCount;
            indexHull = nextHullIndex;

            // Conclude algorithm upon wrap-around
            if (nextHullIndex == rightMost)
            {
                m_vertexCount = outCount;
                break;
            }
        }

        // Copy vertices into shape's vertices
        for (int i = 0; i < m_vertexCount; ++i)
            m_vertices[i] = vertices[hull[i]];

        // Compute face normals
        for (int i1 = 0; i1 < m_vertexCount; ++i1)
        {
            int i2 = i1 + 1 < m_vertexCount ? i1 + 1 : 0;
            Vec face = m_vertices[i2] - m_vertices[i1];

            // Ensure no zero-length edges, because that's bad
            assert(face.squared_vec_length() > EPSILON * EPSILON);

            // Calculate normal with 2D cross product between vector and scalar
            m_normals[i1] = Vec(face.y, -face.x);
            m_normals[i1].Normalize();
        }
    }

    // The extreme point along a direction within a polygon
    Vec GetSupport(const Vec &dir)
    {
        double bestProjection = -FLT_MAX;
        Vec bestVertex;

        for (int i = 0; i < m_vertexCount; ++i)
        {
            Vec v = m_vertices[i];
            double projection = Dot(v, dir);

            if (projection > bestProjection)
            {
                bestVertex = v;
                bestProjection = projection;
            }
        }

        return bestVertex;
    }

    int m_vertexCount;
    Vec m_vertices[MaxPolyVertexCount];
    Vec m_normals[MaxPolyVertexCount];
};

#endif // SHAPE_H
