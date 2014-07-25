//------------------------------------------------------------------------------------------------------------------
template <class Real>
HRGeometry<Real>::HRGeometry (GeometryType eGeometry)
    :
    Geometry(eGeometry),
    Vertices2(0),
    Vertices3(0),
    Vertices4(0),
    RgbColors(0),
    RgbaColors(0),
    SecondaryColors(0),
    Normals(0),
    Materials(0),
    TexCoords1(0),
    TexCoords2(0),
    TexCoords3(0),
    TexCoords4(0),
    EdgeFlags(0),
    FogCoords(0),
    Index(0)
{
}
//------------------------------------------------------------------------------------------------------------------
template <class Real>
void HRGeometry<Real>::UpdateLocalBound ()
{
    // only one vertex dimension must be present
    assert( (Vertices2 && !Vertices3 && !Vertices4)
        ||  (!Vertices2 && Vertices3 && !Vertices4)
        ||  (!Vertices2 && !Vertices3 && Vertices4) );

    if ( Vertices2 )
    {
        assert( Vertices2->GetQuantity() > 0 );

        const Vector2<Real>& rqVertex = (*Vertices2)[0];

        Real fMinX = rqVertex.X();
        Real fMaxX = rqVertex.X();
        Real fMinY = rqVertex.Y();
        Real fMaxY = rqVertex.Y();
        for (int i = 1; i < Vertices2->GetQuantity(); i++)
        {
            const Vector2<Real>& rqVertex = (*Vertices2)[i];

            if ( rqVertex.X() < fMinX )
                fMinX = rqVertex.X();
            else if ( rqVertex.X() > fMaxX )
                fMaxX = rqVertex.X();
            if ( rqVertex.Y() < fMinY )
                fMinY = rqVertex.Y();
            else if ( rqVertex.Y() > fMaxY )
                fMaxY = rqVertex.Y();
        }

        m_qLocalBoundingBox.Center.X() = (fMinX + fMaxX)/(Real)2.0;
        m_qLocalBoundingBox.Center.Y() = (fMinY + fMaxY)/(Real)2.0;
        m_qLocalBoundingBox.Center.Z() = (Real)0.0;

        m_qLocalBoundingBox.Extent[0] = (fMaxX - fMinX)/(Real)2.0;
        m_qLocalBoundingBox.Extent[1] = (fMaxY - fMinY)/(Real)2.0;
        m_qLocalBoundingBox.Extent[2] = (Real)0.0;
    }
    else
    {
        assert( !(Vertices3 && Vertices3->IsEmpty())
            &&  !(Vertices4 && Vertices4->IsEmpty()) );

        Real fMinX, fMaxX, fMinY, fMaxY, fMinZ, fMaxZ;
        if ( Vertices3 )
        {
            const Vector3<Real>& rqVertex = (*Vertices3)[0];

            fMinX = rqVertex.X();
            fMaxX = rqVertex.X();
            fMinY = rqVertex.Y();
            fMaxY = rqVertex.Y();
            fMinZ = rqVertex.Z();
            fMaxZ = rqVertex.Z();
            for (int i = 1; i < Vertices3->GetQuantity(); i++)
            {
                const Vector3<Real>& rqVertex = (*Vertices3)[i];

                if ( rqVertex.X() < fMinX )
                    fMinX = rqVertex.X();
                else if ( rqVertex.X() > fMaxX )
                    fMaxX = rqVertex.X();
                if ( rqVertex.Y() < fMinY )
                    fMinY = rqVertex.Y();
                else if ( rqVertex.Y() > fMaxY )
                    fMaxY = rqVertex.Y();
                if ( rqVertex.Z() < fMinZ )
                    fMinZ = rqVertex.Z();
                else if ( rqVertex.Z() > fMaxZ )
                    fMaxZ = rqVertex.Z();
            }
        }
        else
        {
            const Vector4<Real>& rqVertex = (*Vertices4)[0];

            fMinX = rqVertex.X();
            fMaxX = rqVertex.X();
            fMinY = rqVertex.Y();
            fMaxY = rqVertex.Y();
            fMinZ = rqVertex.Z();
            fMaxZ = rqVertex.Z();
            for (int i = 1; i < Vertices4->GetQuantity(); i++)
            {
                const Vector4<Real>& rqVertex = (*Vertices4)[i];

                if ( rqVertex.X() < fMinX )
                    fMinX = rqVertex.X();
                else if ( rqVertex.X() > fMaxX )
                    fMaxX = rqVertex.X();
                if ( rqVertex.Y() < fMinY )
                    fMinY = rqVertex.Y();
                else if ( rqVertex.Y() > fMaxY )
                    fMaxY = rqVertex.Y();
                if ( rqVertex.Z() < fMinZ )
                    fMinZ = rqVertex.Z();
                else if ( rqVertex.Z() > fMaxZ )
                    fMaxZ = rqVertex.Z();
            }
        }

        m_qLocalBoundingBox.Center.X() = (fMinX + fMaxX)/(Real)2.0;
        m_qLocalBoundingBox.Center.Y() = (fMinY + fMaxY)/(Real)2.0;
        m_qLocalBoundingBox.Center.Z() = (fMinZ + fMaxZ)/(Real)2.0;

        m_qLocalBoundingBox.Extent[0] = (fMaxX - fMinX)/(Real)2.0;
        m_qLocalBoundingBox.Extent[1] = (fMaxY - fMinY)/(Real)2.0;
        m_qLocalBoundingBox.Extent[2] = (fMaxZ - fMinZ)/(Real)2.0;
    }
}
//------------------------------------------------------------------------------------------------------------------



