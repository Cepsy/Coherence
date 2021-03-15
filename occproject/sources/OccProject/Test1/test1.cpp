#include <iostream>
#include <iomanip>
#include "BRepPrimAPI_MakeCylinder.hxx"
#include "BRepPrimAPI_MakeBox.hxx"
#include "BRepAlgoAPI_Cut.hxx"
#include "BRepGProp.hxx"
#include "STEPControl_Writer.hxx"
#include "GProp_GProps.hxx"
#include "TopExp_Explorer.hxx"
#include "TopAbs_ShapeEnum.hxx"
#include "TopoDS.hxx"
#include <vector>
#include "BRepBuilderAPI.hxx"


int main(int argc, char *argv[])
{
	

	//Create a simple box with a size 100x100x50, centered around the origin
	gp_Pnt lowerLeftCornerOfBox(-50.0,-50.0,0.0);
 	BRepPrimAPI_MakeBox boxMaker(lowerLeftCornerOfBox,100,100,100);
	TopoDS_Shape box = boxMaker.Shape();

	int nb_vertex = 0;

	double minixe = 99999;
	double minigrec = 99999;
	double minZ = 99999;
	double maxixe = -99999;
	double maxigrec = -99999;
	double maxZ = -99999;
	std::vector<gp_Pnt> boundingBoite;


	//exploration des vertices pour création de la latice (boundingBox)
	std::vector<gp_Pnt> points; //enregistre les coordonnées 
	for( TopExp_Explorer expVert(box, TopAbs_VERTEX) ; expVert.More() ; expVert.Next() ){
        TopoDS_Vertex vert = TopoDS::Vertex(expVert.Current());

        /* exemple de transformation (translation)
		gp_Vec vecMove(1, 2, 0);
        gp_Trsf transfo;
        transfo.SetTranslation(vecMove);
        TopLoc_Location Loc(transfo);
        vert.Location(Loc); */

        gp_Pnt pnt = BRep_Tool::Pnt(vert); //traduction en gp_Pnt pour acceder aux coordonnées
        
		bool found = false;

		//recherche si déjà parcouru (l'exploreur itère plusieurs fois sur le même vertex)
        for(int i = 0; i < nb_vertex; i++) {
            if(pnt.IsEqual(points.at(i), 0.01)) {
                std::cout << "Element found" << std::endl;
                found = true;
                break;
            }
        }

		//si premiere itération
        if(!found) {
            std::cout << "New Element Found" << std::endl;
            std::cout << pnt.X() << pnt.Y() << pnt.Z() << std::endl;
            points.push_back(pnt);
            nb_vertex++;
			//update des values pour la future latice
			minixe = std::min(double(pnt.X()), minixe);
    		minigrec = std::min(double(pnt.Y()), minigrec);
    		minZ = std::min(double(pnt.Z()), minZ);
    		maxixe = std::max(double(pnt.X()), maxixe);
    		maxigrec = std::max(double(pnt.Y()), maxigrec);
    		maxZ = std::max(double(pnt.Z()), maxZ);


			//test modification :
			if(nb_vertex == 1){
				gp_Vec vecMove(1, 2, 0);
        		gp_Trsf transfo;
        		transfo.SetTranslation(vecMove);
        		TopLoc_Location Loc(transfo);
        		vert.Location(Loc);
			}
        }

        std::cout << "Vertex Processed" << std::endl;
    }

	std::cout << "nb_vertex : " << nb_vertex << std::endl;
	//std::cout << minixe << minigrec << minZ << maxixe << maxigrec << maxZ << std::endl;
	//bounding box simple
	boundingBoite.push_back(gp_Pnt(minixe, minigrec, minZ));
	boundingBoite.push_back(gp_Pnt(maxixe, minigrec, minZ));
	boundingBoite.push_back(gp_Pnt(maxixe, maxigrec, minZ));
	boundingBoite.push_back(gp_Pnt(minixe, maxigrec, minZ));
	boundingBoite.push_back(gp_Pnt(minixe, minigrec, maxZ));
	boundingBoite.push_back(gp_Pnt(maxixe, minigrec, maxZ));
	boundingBoite.push_back(gp_Pnt(maxixe, maxigrec, maxZ));
	boundingBoite.push_back(gp_Pnt(minixe, maxigrec, maxZ));
	boundingBoite.push_back(gp_Pnt(minixe, maxigrec, maxZ));

	//latice plus complexe 
	boundingBoite.push_back(gp_Pnt(minixe, minigrec, (minZ+maxZ)/2));
	boundingBoite.push_back(gp_Pnt(minixe, (minigrec+maxigrec)/2, minZ));
	boundingBoite.push_back(gp_Pnt((minixe+maxixe)/2, minigrec, minZ));
	boundingBoite.push_back(gp_Pnt(maxixe, (minigrec+maxigrec)/2, minZ));
	boundingBoite.push_back(gp_Pnt(maxixe, minigrec, (minZ+maxZ)/2));
	boundingBoite.push_back(gp_Pnt((minixe+maxixe)/2, maxigrec, minZ));
	boundingBoite.push_back(gp_Pnt(minixe, maxigrec, (minZ+maxZ)/2));
	boundingBoite.push_back(gp_Pnt((minixe+maxixe)/2, minigrec, maxZ));
	boundingBoite.push_back(gp_Pnt(minixe, (minigrec+maxigrec)/2, maxZ));
	boundingBoite.push_back(gp_Pnt((maxixe+minixe)/2, maxigrec, maxZ));
	boundingBoite.push_back(gp_Pnt(maxixe, (minigrec+maxigrec)/2, maxZ));
	boundingBoite.push_back(gp_Pnt(maxixe, maxigrec, (maxZ+minZ)/2));





	std::cout << "nb_vertex : " << nb_vertex << std::endl;

	/*
	//Create a cylinder with a radius 25.0 and height 50.0, centered at the origin 
 	BRepPrimAPI_MakeCylinder cylinderMaker(25.0,50.0);
	TopoDS_Shape cylinder = cylinderMaker.Shape();

	//Cut the cylinder out from the box
	BRepAlgoAPI_Cut cutMaker(box,cylinder);
	TopoDS_Shape boxWithHole = cutMaker.Shape();
	
	/*
	//Write the resulting shape to a file
	STEPControl_Writer writer;
	writer.Transfer(cylinder,STEPControl_AsIs);
	writer.Write("cylinder.stp");
	writer.Transfer(box,STEPControl_AsIs);
	writer.Write("box.stp");
	writer.Transfer(boxWithHole,STEPControl_AsIs);
	writer.Write("boxWithHole.stp");

 	std::cout << "Created box with hole, file is written to boxWithHole.stp" << std::endl;	
	*/

	/*
	//We compute some volumetric properties of the resulting shape
        GProp_GProps volumeProperties;
	BRepGProp::VolumeProperties(boxWithHole,volumeProperties);

	//Compute the volume of the model
	std::cout << std::setprecision(14) << "Volume of the model is: " << volumeProperties.Mass() << std::endl;
	
	//Compute the center of mass
	std::cout << "Center of mass is: " << volumeProperties.CentreOfMass().X() << " " << volumeProperties.CentreOfMass().Y() << " " << volumeProperties.CentreOfMass().Z() << std::endl;

	//Compute the matrix of inertia
	gp_Mat inertiaMatrix = volumeProperties.MatrixOfInertia();
	std::cout << "Matrix of inertia: " << std::endl;
	for (int i = 1; i <= 3; ++i) {
		for (int j = 1; j <= 3; ++j) {
			std::cout << inertiaMatrix(i,j) << "\t";
		}
		std::cout << std::endl;
	}*/

	return 0;
}
