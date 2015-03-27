// JdeRobot C++ Labyrinth challenge solution by Ilkka Laine

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <queue>

/*
Note: this algorithm will not find the longest path in labyrinth with wider open spaces.
In such a case, finding the longest path is a much harder problem.
*/
/*
Pathfinding algorithm

Path lengths are determined using the following steps:
Step 1. Find next hole. (moving left to right and top to bottom)
Step 2. Mark it with number 0.
Step 3. Mark adjacent(unmarked) holes with previous number + 1.
Step 4. Repeat step 3 for all marked holes.
Step 5. Repeat steps 1-4 until there are no more holes.

The longest path can then be found by finding the biggest number and then traversing that path backwards.
*/


int markPath(size_t startIndex, size_t width, size_t height, std::string input, std::vector<int>& visited){
	size_t ind = startIndex;	
	int distance = 0;
	visited[ind] = 0;
	
	
	//find and mark adjacent unvisited holes
	std::queue<size_t> adjacents;
	std::queue<size_t> distancesToAdjacents;
	adjacents.push(ind); //starting point is put into adjacents to initialize loop
	distancesToAdjacents.push(distance);//so is starting distance 
	while(!adjacents.empty()){
		ind = adjacents.front();
		adjacents.pop();
		distance = distancesToAdjacents.front();
		distancesToAdjacents.pop();
		//right
		if((input[ind+1] == '.') && (visited[ind+1] == -1)){  //not on right edge and not a wall and not visited
			adjacents.push(ind+1);
			distancesToAdjacents.push(distance+1);
			visited[ind+1] = distance+1;
		}
		//left
		if((ind != 0) && (input[ind-1] == '.') && (visited[ind-1] == -1)){
			adjacents.push(ind-1);
			distancesToAdjacents.push(distance+1);
			visited[ind-1] = distance+1;
		}
		//up
		if((ind > width+1) && (input[ind - (width+1)] == '.') && (visited[ind - (width+1)] == -1)){
			adjacents.push(ind - (width+1));
			distancesToAdjacents.push(distance+1);
			visited[ind - (width+1)] = distance+1;
		}
		//down
		if((ind < input.length() - (width+1)) && (input[ind + (width+1)] == '.') && (visited[ind + (width+1)] == -1)){
			adjacents.push(ind + (width+1));
			distancesToAdjacents.push(distance+1);
			visited[ind + (width+1)] = distance+1;
		}

	}

	return distance+1;
}


int main(int argc, char* argv[]){ //takes filename as a command line argument 
	if(argc < 2){
		std::cout << "Please provide a file name." << std::endl;
		return 0;
	}


	//read from text file and put the contents in a string
	std::ifstream ifs(argv[1]);  
	if (!ifs){
		std::cout << "Unable to open the file." << std::endl;
		return 1;
	}
	std::ostringstream contents;
	contents << ifs.rdbuf();
	ifs.close();
	std::string input = contents.str(); // input is original data
	

	//find size
	size_t width = 0;
	size_t height = 0;
	size_t size = 0;
	bool widthfound = false;
	for(size_t i = 0;input[i]; i++){
		if(input[i] == '#' || input[i] == '.'){  //wall or hole
			size++;
			if(!widthfound)  //first line determines width
				width++;
		}
		else if(input[i] == '\n'){ //line break
			widthfound = true;
			continue;
		}
		else //any other characters cause an error
			throw std::runtime_error("Invalid input.");  
	}
	height = size/width;
	//std::cout << "size: " << size << "\nwidth: " << width << "\nheight: " << height <<"\n";

	
	//Visited holes are stored in a vector<int>. Value -1 means non-visited.
	std::vector<int> visited;
	visited.push_back(-1);
	visited.resize(input.length() +1,-1); //It's made the same length as input (containing values for linebreaks) for practical indexing.


	//find next unmarked hole
	for(size_t y = 0; y < height; y++){
		for(size_t x = 0; x < width; x++){
			size_t ind = x + y * (width +1);
			if(input[ind] == '#'){
			}
			else if(input[ind] == '.'){
				if(visited[ind] == -1)
					markPath(ind,width,height,input,visited); //marks path length
			}
			else{
				throw std::runtime_error("Invalid input."); //could be checked when reading the file?
			}
		}
	}


	//determine the longest path and create output
	std::string output = input;
	int longestPathLength = 0;
	size_t longestPathIndex = 0;
	for(size_t i = 0; i < visited.size(); i++){
		//if(output[i] =='.') {output[i] = (char) ('0' + (visited[i] % 10));}  //for printing all paths
		if((visited[i] +1) > longestPathLength){
			longestPathLength = visited[i] +1;
			longestPathIndex = i;
		}
	}
	output[longestPathIndex] = (char) ('0' + (visited[longestPathIndex]) % 10);
	size_t ind = longestPathIndex;
	for(int nextDistance = longestPathLength-2; nextDistance >= 0; nextDistance--){
		//right
		if(visited[ind+1] == nextDistance)  
			ind = ind+1;
		//left
		else if(visited[ind-1] == nextDistance)
			ind = ind-1;
		//up
		else if((ind > width+1) && (visited[ind - (width+1)] == nextDistance))
			ind = ind - (width+1);
		//down
		else if((ind < input.length() - (width+1)) && (visited[ind + (width+1)] == nextDistance))
			ind = ind + (width+1);
		else{
			std::cout << "Something went wrong.(in main in longest path travel loop)\n";
		}
		output[ind] = (char) ('0' + (nextDistance % 10)); //prints the last digit from numbers bigger than 10
	}
	

	//print results
	std::cout << longestPathLength <<std::endl;
	std::cout << output;

	return 0;
}



