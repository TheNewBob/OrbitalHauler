#include "core/Common.h"
#include "Rotations.h"
#include "Calc.h"

Rotations::Rotations()
{
}


Rotations::~Rotations()
{
}

//returns a unit matrix
MATRIX3 Rotations::UnitMatrix()
{
	return _M(1, 0, 0,
			 0, 1, 0,
			 0, 0, 1);
}


MATRIX3 Rotations::GetRotationMatrixFromDirection(VECTOR3 dir)
{
	MATRIX3 dirmat;
	VECTOR3 unitRot = _V(0, 1, 0);
	if (!RequiresRotation(dir, unitRot))
	{
		return UnitMatrix();
	}

	if (dir.x == 0 && dir.y == 0 && dir.z == 1)
		//dir is origin, no transformation of dir required, define unit matrix
	{
		dirmat = UnitMatrix();
	}
	else
	{
		VECTOR3 dirAxis;				//axis of rotation for directional vector
		if (dir.x == 0 && dir.y == 0 && dir.z == -1)
		{
			//corner case for inversed direction. Would result in division by zero if processed the normal way
			dirAxis = _V(0, 1, 0);
		}
		else
		{
			dirAxis = crossp(_V(0, 0, 1), dir);
		}

		Calc::RoundVector(dirAxis, 1000);

		//get the rotation angle
		double dirAngle = Rotations::GetAngle(_V(0, 0, 1), dir, dirAxis);
		//build matrix to transform to directional vector
		Rotations::BuildRotationMatrix(dirmat, dirAxis, dirAngle);
	}
	return dirmat;
}


//returns a rotation matrix that will rotate two vectors (0,0,1)(0,1,0) to dir and rot.
//dir and rot must be perpendicular, obviously, and normalised.
MATRIX3 Rotations::GetRotationMatrixFromOrientation(VECTOR3 dir, VECTOR3 rot)
{
	MATRIX3 transformation;
	MATRIX3 dirmat;
	if (!RequiresRotation(dir, rot))
	{
		return UnitMatrix();
	}

	bool needsSecondTransformation = true;
	VECTOR3 newrot = _V(0, 1, 0);
	if (dir.x == 0 && dir.y == 0 && dir.z == 1)
	//dir is origin, no transformation of dir required, define unit matrix
	{
		dirmat = UnitMatrix();
	}
	else
	{
		VECTOR3 dirAxis;				//axis of rotation for directional vector
		if (dir.x == 0 && dir.y == 0 && dir.z == -1)
		//corner case for inversed direction. Would result in division by zero if processed the normal way
		{
			dirAxis = _V(0, 1, 0);
		}
		else
		{
			dirAxis = crossp(_V(0, 0, 1), dir);
		}

		Calc::RoundVector(dirAxis, 1000);
		//normalise(dirAxis);										//roundi

		//get the rotation angle
		double dirAngle = Rotations::GetAngle(_V(0, 0, 1), dir, dirAxis);
		//build matrix to transform to directional vector
		Rotations::BuildRotationMatrix(dirmat, dirAxis, dirAngle);
		//calculate the rotational vector after the first transformation was applied
		newrot = mul(dirmat, _V(0, 1, 0));						
		//round and normalise for easy comparison
		Calc::RoundVector(newrot, 1000);
		normalise(newrot);
		//check if the rotation ended up right, or if we need another transformation
		if (newrot.x == rot.x && newrot.y == rot.y && newrot.z == rot.z)
		{
			needsSecondTransformation = false;
		}
	}

	if (needsSecondTransformation)
	{
		MATRIX3 rotmat;
		double rotAngle = Rotations::GetAngle(newrot, rot, dir);
		Rotations::BuildRotationMatrix(rotmat, dir, rotAngle);
		transformation = mul(rotmat, dirmat);
	}
	else
	{
		transformation = dirmat;
	}

	return transformation;
}


//returns false if the passed vectors are already at origin ( 0,0,1 / 0,1,0 )
bool Rotations::RequiresRotation(VECTOR3 &IN_dir, VECTOR3 &IN_rot)
{
	if (!Calc::IsEqual(IN_dir.x, 0) || !Calc::IsEqual(IN_dir.y, 0) || !Calc::IsEqual(IN_dir.z, 1) ||
		!Calc::IsEqual(IN_rot.x, 0) || !Calc::IsEqual(IN_rot.y, 1) || !Calc::IsEqual(IN_rot.z, 0))
	{
		return true;
	}

	return false;
}



void Rotations::TransformMesh(MESHHANDLE mshTemplate, DEVMESHHANDLE hDmesh, MATRIX3 &rotationmatrix)
//rotates a mesh to its alignement. Replacement of Meshgroup_Transform for client compatibility
{
	DWORD mshGroupCount = oapiMeshGroupCount(mshTemplate);
	for (DWORD i = 0; i < mshGroupCount; ++i)
	{
		MESHGROUP* group = oapiMeshGroup(mshTemplate, i);
		GROUPEDITSPEC groupEdit;
		groupEdit.nVtx = group->nVtx;

		//preparing groupEdit for data sotrage and calculation
		groupEdit.Vtx = new NTVERTEX[groupEdit.nVtx];
		groupEdit.vIdx = new WORD[groupEdit.nVtx];
		groupEdit.flags = (GRPEDIT_VTXCRD | GRPEDIT_VTXNML);		//need to replace coordinates and normals

		for (DWORD j = 0; j < groupEdit.nVtx; ++j)
		{
			//rotating vertex positions
			VECTOR3 newPos = _V(group->Vtx[j].x,
				group->Vtx[j].y,
				group->Vtx[j].z);
			VECTOR3 finalPos = mul(rotationmatrix, newPos);

			//rotating normals
			VECTOR3 newNorms = _V(group->Vtx[j].nx,
				group->Vtx[j].ny,
				group->Vtx[j].nz);
			VECTOR3 finalNorms = mul(rotationmatrix, newNorms);

			//saving new data to groupEdit
			groupEdit.Vtx[j].x = float(finalPos.x);
			groupEdit.Vtx[j].y = float(finalPos.y);
			groupEdit.Vtx[j].z = float(finalPos.z);

			groupEdit.Vtx[j].nx = float(finalNorms.x);
			groupEdit.Vtx[j].ny = float(finalNorms.y);
			groupEdit.Vtx[j].nz = float(finalNorms.z);

			//groupEdit.vIdx[j] = group->Idx[j];	//we have to replace all the jices
			groupEdit.vIdx[j] = WORD(j);

		}
		oapiEditMeshGroup(hDmesh, i, &groupEdit);
		//cleaning up
		delete groupEdit.vIdx;
		delete groupEdit.Vtx;
	}
}


void Rotations::BuildRotationMatrix(MATRIX3& result, const VECTOR3& rotationAxis, double angle)
{
	double c = cos(angle);
	double s = sin(angle);
	double t = 1.0 - c;
	double x = rotationAxis.x, y = rotationAxis.y, z = rotationAxis.z;
	result = _M(t * x * x + c, t * x * y - s * z, t * x * z + s * y,
		t * x * y + s * z, t * y * y + c, t * y * z - s * x,
		t * x * z - s * y, t * y * z + s * x, t * z * z + c);
}


void Rotations::TransformMesh(MESHHANDLE mshTemplate, DEVMESHHANDLE hDmesh, MATRIX3 &firstRot, MATRIX3 &secondRot)
//rotates a mesh to its alignement. Replacement of Meshgroup_Transform for client compatibility
{
	DWORD mshGroupCount = oapiMeshGroupCount(mshTemplate);
	for (DWORD i = 0; i < mshGroupCount; ++i)
	{
		MESHGROUP* group = oapiMeshGroup(mshTemplate, i);
		GROUPEDITSPEC groupEdit;
		groupEdit.nVtx = group->nVtx;

		//preparing groupEdit for data sotrage and calculation
		groupEdit.Vtx = new NTVERTEX[groupEdit.nVtx];
		groupEdit.vIdx = new WORD[groupEdit.nVtx];
		groupEdit.flags = (GRPEDIT_VTXCRD | GRPEDIT_VTXNML);		//need to replace coordinates and normals

		for (DWORD j = 0; j < groupEdit.nVtx; ++j)
		{
			//rotating vertex positions
			VECTOR3 newPos = _V(group->Vtx[j].x,
				group->Vtx[j].y,
				group->Vtx[j].z);
			newPos = mul(firstRot, newPos);
			VECTOR3 finalPos = mul(secondRot, newPos);

			//rotating normals
			VECTOR3 newNorms = _V(group->Vtx[j].nx,
				group->Vtx[j].ny,
				group->Vtx[j].nz);
			newNorms = mul(firstRot, newNorms);
			VECTOR3 finalNorms = mul(secondRot, newNorms);

			//saving new data to groupEdit
			groupEdit.Vtx[j].x = float(finalPos.x);
			groupEdit.Vtx[j].y = float(finalPos.y);
			groupEdit.Vtx[j].z = float(finalPos.z);

			groupEdit.Vtx[j].nx = float(finalNorms.x);
			groupEdit.Vtx[j].ny = float(finalNorms.y);
			groupEdit.Vtx[j].nz = float(finalNorms.z);

			//groupEdit.vIdx[j] = group->Idx[j];	//we have to replace all the jices
			groupEdit.vIdx[j] = WORD(j);

		}
		oapiEditMeshGroup(hDmesh, i, &groupEdit);
		//cleaning up
		delete groupEdit.vIdx;
		delete groupEdit.Vtx;
	}
}


//returns the inverse of a rotation matrix
MATRIX3 Rotations::InverseMatrix(MATRIX3 matrix)
{
	return _M(matrix.m11, matrix.m21, matrix.m31,
		matrix.m12, matrix.m22, matrix.m32,
		matrix.m13, matrix.m23, matrix.m33);
}


double Rotations::GetAngle(const VECTOR3& v1, const VECTOR3& v2, const VECTOR3& signRef)
{
	double angle = (float)acos(dotp(v1, v2) / (length(v1) * length(v2)));
	if (dotp(signRef, crossp(v1, v2)) < 0)
	{
		angle = -angle;
	}
	return angle;
}


void Rotations::RotateVector(VECTOR3& v, const VECTOR3& axis, const double angle) {
	double c = cos(angle); double s = sin(angle); double C = 1 - c;
	double xs = axis.x * s; double ys = axis.y * s; double zs = axis.z * s;
	double xC = axis.x * C; double yC = axis.y * C; double zC = axis.z * C;
	double xyC = axis.x * yC; double yzC = axis.y * zC; double zxC = axis.z * xC;
	MATRIX3 m = _M(
		axis.x * xC + c, xyC - zs, zxC + ys,
		xyC + zs, axis.y * yC + c, yzC - xs,
		zxC - ys, yzC + xs, axis.z * zC + c);
	v = mul(m, v);
}


VECTOR3 Rotations::GetProjection(const VECTOR3& v, const VECTOR3& planeNormal) 
{
	// projection of v1 on v2 = v2 * (dotp(v1, v2) / |v2|^2)
	return v - planeNormal * (dotp(v, planeNormal) / pow(length(planeNormal), 2));
}