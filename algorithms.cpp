#include "algorithms.h"

#define BOUNDINGBOX 1e+6

namespace Algorithms{
/**
 * @brief BuildTrapezoidalMap, performe the incremental step.
 * Given the tMap, the dag and a segment, validate the segment (if the p1 x is bigger than p2 swap p1 and p2),
 * insert the segment in the trapezoidal map obtaining the ids of the trapezoid intersected by that segment, and for each trapezoid
 * intersected check the configuration segment/current trapezoid to decide which type of case is and do the update accordingly to that.
 * The update is done first in the trapezoidal map, doing the split and the eventual merge of the trapezoids, and then using the vector
 * lastTrapezoidsInserted to update the dag.
 * @param tMap
 * @param dag
 * @param segment, the segment inserted in the trapezoidal map.
 * @note degenerative can become true only if is intersected only 1 trapezoid and the one point of the current trapezoid is shared with the segment
 * check ifs
 */
void buildTrapezoidalMap(TrapezoidalMap& tMap, Dag& dag, cg3::Segment2d segment){

    /*Validate segments*/
    geoutils::validateSegment(segment);

    std::vector<Node> nodes = std::vector<Node>();
    bool degenerative = false;
    std::vector<size_t> trapsIntersected = std::vector<size_t>();
    std::vector<size_t> lastTrapezoidsInserted = std::vector<size_t>();

    followSegment(tMap, dag, segment, trapsIntersected);

    nodes.push_back(Node(xNode, tMap.addPoint(segment.p1())));
    nodes.push_back(Node(yNode, tMap.addSegment(segment)));
    nodes.push_back(Node(xNode, tMap.addPoint(segment.p2())));

    /*Performe splitting and neighbor fixing according to case*/
    for(size_t i = 0; i < trapsIntersected.size(); i++){
        Trapezoid currentT = tMap.trapezoidcpy(trapsIntersected[i]);
        /*DEGENERATIVE: P1 is INSIDE currentT, Q1 is EQUALT to rightp, 1 Trapezoid Intersected*/
        if (segment.p1().x() > currentT.leftp().x() and (geoutils::arePointsEqual(segment.p2(),currentT.top().p2()) or geoutils::arePointsEqual(segment.p2(),currentT.bottom().p2()) or geoutils::arePointsEqual(segment.p2(), currentT.rightp()))){
            degenerative = true;
            handleCaseSegmentInsideDegenerativeRight(tMap, trapsIntersected[i], segment, lastTrapezoidsInserted);
            performeDagUpdate(tMap, dag, currentT, segment, nodes, lastTrapezoidsInserted, degenerative);
            degenerative = false;
            /*DEGENERATIVE: P1 is EQUAL leftp, P2 is INSIDE currentT, 1 Trapezoid Intersected*/
        }else if((geoutils::arePointsEqual(segment.p1(),currentT.top().p1()) or geoutils::arePointsEqual(segment.p1(),currentT.bottom().p1()) or geoutils::arePointsEqual(segment.p1(), currentT.leftp())) and segment.p2().x() < currentT.rightp().x()){
            degenerative = true;
            handleCaseSegmentInsideDegenerativeLeft(tMap, trapsIntersected[i], segment, lastTrapezoidsInserted);
            performeDagUpdate(tMap, dag, currentT, segment, nodes, lastTrapezoidsInserted, degenerative);
            degenerative = false;
        }else if(segment.p1().x() < currentT.leftp().x() and geoutils::arePointsEqual(segment.p2(), currentT.rightp())){
            handleCaseQ1InsideDegenerative(tMap, trapsIntersected[i], segment, lastTrapezoidsInserted);
            performeDagUpdate(tMap, dag, currentT, segment, nodes, lastTrapezoidsInserted, degenerative);
        }else if(geoutils::arePointsEqual(segment.p1().x(),currentT.leftp().x()) and segment.p2().x() > currentT.rightp().x()){
            handleCaseP1InsideDegenerative(tMap, trapsIntersected[i], segment, lastTrapezoidsInserted);
            performeDagUpdate(tMap, dag, currentT, segment, nodes, lastTrapezoidsInserted, degenerative);
        }
        /*P1 is INSIDE currentT, Q1 is INSIDE currentT*/
        else if(segment.p1().x() >= currentT.leftp().x() and segment.p2().x() <= currentT.rightp().x()){
            handleCaseSegmentInside(tMap, trapsIntersected[0], segment, lastTrapezoidsInserted);
            performeDagUpdate(tMap, dag, currentT, segment, nodes, lastTrapezoidsInserted, degenerative);
            /*P1 is INSIDE currentT, Q1 is OUTSIDE currentT*/
        }else if (segment.p1().x() > currentT.leftp().x() and segment.p2().x() > currentT.rightp().x()){
            handleCaseP1Inside(tMap, trapsIntersected[i], segment, lastTrapezoidsInserted);
            performeDagUpdate(tMap, dag, currentT, segment, nodes, lastTrapezoidsInserted, degenerative);
            /*P1 is OUTSIDE currentT, Q1 is OUTSIDE currentT*/
        }else if(segment.p1().x() < currentT.leftp().x() and segment.p2().x() > currentT.rightp().x()){
            handleCasePointsOutside(tMap, trapsIntersected[i], segment, lastTrapezoidsInserted);
            performeDagUpdate(tMap, dag, currentT, segment, nodes, lastTrapezoidsInserted, degenerative);
            /*P1 is OUTSIDE currentT, Q1 is INSIDE currentT*/
        }else if(segment.p1().x() < currentT.leftp().x() and segment.p2().x() < currentT.rightp().x()){
            handleCaseQ1Inside(tMap, trapsIntersected[i], segment, lastTrapezoidsInserted);
            performeDagUpdate(tMap, dag, currentT, segment, nodes, lastTrapezoidsInserted, degenerative);
        }
    }


};

/**
 * @brief Handle the case when the segment is totally inside a trapezoid.
 * Case when the segment is fully inside the trapezoid intersected, the case will split the trapezoid in four new trapezoid,
 * substituting the old one with one of the new, and updating their references.
 * @param currentId, id of the trapezoid to split in the map.
 * @param segment, segment which is crossing the current trapezoid.
 * @param lastTrapezoidsInserted, vector that will store the ids of the newly added segment in order to insert them in the dag.
 * @return The four trapezoid obtained by the split with the adjacency updated, pushed into the vector lastTrapezoidsInserted.
 */
void handleCaseSegmentInside(TrapezoidalMap& tMap, const size_t& currentId, const cg3::Segment2d& segment, std::vector<size_t>& lastTrapezoidsInserted){
    Trapezoid current = tMap.trapezoidcpy(currentId);
    tMap.SplitInFour(currentId, segment, lastTrapezoidsInserted);

    size_t idA = lastTrapezoidsInserted[0];
    size_t idB = lastTrapezoidsInserted[1];
    size_t idC = lastTrapezoidsInserted[2];
    size_t idD = lastTrapezoidsInserted[3];

    Trapezoid& referenceA = tMap.trapezoid(idA);
    Trapezoid& referenceB = tMap.trapezoid(idB);
    Trapezoid& referenceC = tMap.trapezoid(idC);
    Trapezoid& referenceD = tMap.trapezoid(idD);

    referenceA.updateNeighborsRight(current, idB, idC);
    referenceB.updateNeighbors(idA, idD);
    referenceC.updateNeighbors(idA, idD);
    referenceD.updateNeighborsLeft(current, idB, idC);
    tMap.indirectUpdateNeighbors(current, currentId, true, idA);
    tMap.indirectUpdateNeighbors(current, currentId, false, idD);
}
/**
 * @brief Handle the case when the segment is totally inside a trapezoid and the right endpoint of the segment is the leftp or
 * the p1 of the top or the p1 of the bottom.
 * @param currentId, id of the trapezoid to split in the map.
 * @param segment, segment which is interesecting the current trapezoid.
 * @param lastTrapezoidsInserted, vector that will store the ids of the newly added segment in order to insert them in the dag.
 * @return The three trapezoid obtained by the split with the adjacency updated, pushed into the vector lastTrapezoidsInserted.
 */
void handleCaseSegmentInsideDegenerativeRight(TrapezoidalMap& tMap, const size_t &currentId, const cg3::Segment2d& segment, std::vector<size_t>& lastTrapezoidsInserted){
    Trapezoid current = tMap.trapezoidcpy(currentId);
    lastTrapezoidsInserted.clear();
    tMap.SplitInThree(currentId, segment, segment.p1(), lastTrapezoidsInserted);

    /*Add new trapezoid on the map*/
    size_t idLeft = lastTrapezoidsInserted[0];
    size_t idRightUpper = lastTrapezoidsInserted[1];
    size_t idRightLower = lastTrapezoidsInserted[2];

    bool isQ1EqualTopP2 = geoutils::arePointsEqual(segment.p2(), current.top().p2());
    bool isQ1EqualBotP2 = geoutils::arePointsEqual(segment.p2(), current.bottom().p2());

    Trapezoid& referenceLeft = tMap.trapezoid(idLeft);
    Trapezoid& referenceRightU = tMap.trapezoid(idRightUpper);
    Trapezoid& referenceRightL = tMap.trapezoid(idRightLower);

    /*Set neighbors internally*/
    referenceLeft.updateLeftNeighborsOld(current);
    referenceLeft.updateRightNeighbors(idRightUpper, idRightLower);
    referenceRightU.updateLeftNeighbors(idLeft);
    referenceRightL.updateLeftNeighbors(idLeft);

    /*Set neighbors depending by the case*/
    tMap.indirectUpdateNeighbors(current, currentId, true, idLeft);
    if (isQ1EqualTopP2 and !isQ1EqualBotP2){
        tMap.indirectUpdateNeighbors(current, currentId, false, idRightLower);
        referenceRightL.updateRightNeighborsOld(current);
    }else if(!isQ1EqualTopP2 and isQ1EqualBotP2){
        tMap.indirectUpdateNeighbors(current, currentId, false, idRightUpper);
        referenceRightU.updateRightNeighborsOld(current);
    }else{
        referenceRightL.updateRightNeighbors(current.lowerRightNeighbor());
        referenceRightU.updateRightNeighbors(current.upperRightNeighbor());
        if(current.lowerRightNeighbor() != SIZE_MAX){
            tMap.trapezoid(current.lowerRightNeighbor()).updateLeftNeighbors(idRightLower);
        }
        if(current.upperRightNeighbor() != SIZE_MAX){
            tMap.trapezoid(current.upperRightNeighbor()).updateLeftNeighbors(idRightUpper);
        }
    }
}

/**
 * @brief Handle the case when the segment is totally inside a trapezoid and the left endpoint of the segment is the rightp or
 * the p2 of the top or the p2 of the bottom.
 * @param currentId, id of the trapezoid to split in the map.
 * @param segment, segment which is interesecting the current trapezoid.
 * @param lastTrapezoidsInserted, vector that will store the ids of the newly added segment in order to insert them in the dag.
 * @return The three trapezoid obtained by the split with the adjacency updated, pushed into the vector lastTrapezoidsInserted.
 */
void handleCaseSegmentInsideDegenerativeLeft(TrapezoidalMap& tMap, const size_t &currentId, const cg3::Segment2d& segment, std::vector<size_t>& lastTrapezoidsInserted){
    Trapezoid current = tMap.trapezoidcpy(currentId);
    lastTrapezoidsInserted.clear();
    bool isP1EqualTopP1 = geoutils::arePointsEqual(segment.p1(),current.top().p1());
    bool isP1EqualBotP1 = geoutils::arePointsEqual(segment.p1(), current.bottom().p1());
    tMap.SplitInThree(currentId, segment, segment.p2(), lastTrapezoidsInserted);

    /*Add new trapezoid on the map*/
    size_t idRight = lastTrapezoidsInserted[0];
    size_t idLeftUpper = lastTrapezoidsInserted[1];
    size_t idLeftLower = lastTrapezoidsInserted[2];

    Trapezoid& referenceRight = tMap.trapezoid(idRight);
    Trapezoid& referenceLeftU = tMap.trapezoid(idLeftUpper);
    Trapezoid& referenceLeftL = tMap.trapezoid(idLeftLower);

    /*Set neighbors internally for tSplitResult*/
    referenceRight.updateRightNeighborsOld(current);
    referenceRight.updateLeftNeighbors(idLeftUpper, idLeftLower);
    referenceLeftU.updateRightNeighbors(idRight);
    referenceLeftL.updateRightNeighbors(idRight);
    tMap.indirectUpdateNeighbors(current, currentId, false, idRight);

    if(isP1EqualTopP1 and !isP1EqualBotP1){
        tMap.indirectUpdateNeighbors(current, currentId, true, idLeftLower);
        referenceLeftL.updateLeftNeighborsOld(current);
    }else if(!isP1EqualTopP1 and isP1EqualBotP1){
        tMap.indirectUpdateNeighbors(current, currentId, true, idLeftUpper);
        referenceLeftU.updateLeftNeighborsOld(current);
    }else{
        referenceLeftL.updateLeftNeighbors(current.lowerLeftNeighbor());
        referenceLeftU.updateLeftNeighbors(current.upperLeftNeighbor());
        if(current.lowerLeftNeighbor() != SIZE_MAX){
            tMap.trapezoid(current.lowerLeftNeighbor()).updateRightNeighbors(idLeftLower);
        }
        if(current.upperLeftNeighbor() != SIZE_MAX){
            tMap.trapezoid(current.upperLeftNeighbor()).updateRightNeighbors(idLeftUpper);
        }
    }

}
/**
 * @brief Handle the case when the p1 is equal to the leftp of the trapezoid and the q1 is out of the trapezoid.
 * Split the trapezoid in two other trapezoids, push their id in the lastTrapezoidInserted vector, based on the fact that the rightp is
 * above or below the segment push the lower or the upper trapezoid in the elegibleformerge vector. Updates the adjacency for the next
 * step.
 * @param currentId, id of the trapezoid to split in the map.
 * @param segment, segment which is interesecting the current trapezoid.
 * @param lastTrapezoidsInserted, vector that will store the ids of the newly added segment in order to insert them in the dag.
 * @return The two trapezoid obtained by the split with the adjacency updated, pushed into the vector lastTrapezoidsInserted.
 */
void handleCaseP1InsideDegenerative(TrapezoidalMap& tMap, const size_t &currentId, const cg3::Segment2d& segment, std::vector<size_t>& lastTrapezoidsInserted){
    Trapezoid current = tMap.trapezoidcpy(currentId);

    bool isPointAbove = geoutils::isPointAbove(current.rightp(), segment);
    bool isP1EqualTopP1 = geoutils::arePointsEqual(segment.p1(),current.top().p1());
    bool isP1EqualBotP1 = geoutils::arePointsEqual(segment.p1(), current.bottom().p1());

    cg3::Point2d q1 = cg3::Point2d(current.rightp().x(), geoutils::calculateYCoord(segment, current.rightp().x()));
    cg3::Segment2d innerSegment = cg3::Segment2d(segment.p1(), q1);

    std::vector<Trapezoid> hSplit = std::vector<Trapezoid>();
    tMap.SplitHorizontaly(current, innerSegment, hSplit);

    lastTrapezoidsInserted.clear();
    lastTrapezoidsInserted.push_back(tMap.replace(currentId, hSplit[0]));
    lastTrapezoidsInserted.push_back(tMap.addTrapezoid(hSplit[1]));

    size_t idUpper = lastTrapezoidsInserted[0];
    size_t idLower = lastTrapezoidsInserted[1];
    Trapezoid& referenceUpper = tMap.trapezoid(idUpper);
    Trapezoid& referenceLower = tMap.trapezoid(idLower);

    if (isPointAbove){
        tMap.addElegibleForMerge(idLower);
        tMap.indirectUpdateNeighbors(current, currentId, false, idUpper);
        referenceUpper.updateRightNeighborsOld(current);
    }else{
        tMap.addElegibleForMerge(idUpper);
        tMap.indirectUpdateNeighbors(current, currentId, false, idLower);
        referenceLower.updateRightNeighborsOld(current);
    }

    if(isP1EqualTopP1 and !isP1EqualBotP1){
        referenceLower.updateLeftNeighborsOld(current);
        tMap.indirectUpdateNeighbors(current, currentId, true, idLower);
    }else if(!isP1EqualTopP1 and isP1EqualBotP1){
        referenceUpper.updateLeftNeighborsOld(current);
        tMap.indirectUpdateNeighbors(current, currentId, true, idUpper);
    }else if(isP1EqualTopP1 and isP1EqualBotP1){

    }else{
        referenceUpper.updateLeftNeighbors(current.upperLeftNeighbor());
        referenceLower.updateLeftNeighbors(current.lowerLeftNeighbor());
        if (isPointAbove){
            referenceUpper.updateRightNeighborsOld(current);
        }else{
            referenceLower.updateRightNeighborsOld(current);
        }

        if(current.upperLeftNeighbor() != SIZE_MAX){
            tMap.trapezoid(current.upperLeftNeighbor()).updateRightNeighbors(idUpper);
        }
        if(current.lowerLeftNeighbor() != SIZE_MAX){
            tMap.trapezoid(current.lowerLeftNeighbor()).updateRightNeighbors(idLower);
        }
    }
}
/**
 * @brief Handle the case when the q1 is equal to the rightp of the trapezoid and the p1 is out of the trapezoid.
 * Split the trapezoid in two other trapezoids, push their id in the lastTrapezoidInserted vector, based on the fact that the rightp is
 * above or below the segment push the lower or the upper trapezoid in the elegibleformerge vector. Completes the update of the adjacency,
 * based on the boolean isQ1EqualTopP2 and boolean isQ1EqualBotP2.
 * @param currentId, id of the trapezoid to split in the map.
 * @param segment, segment which is interesecting the current trapezoid.
 * @param lastTrapezoidsInserted, vector that will store the ids of the newly added segment in order to insert them in the dag.
 * @return The two trapezoid obtained by the split with the adjacency updated, pushed into the vector lastTrapezoidsInserted.
 */
void handleCaseQ1InsideDegenerative(TrapezoidalMap& tMap, const size_t &currentId, const cg3::Segment2d& segment, std::vector<size_t>& lastTrapezoidsInserted){
    Trapezoid current = tMap.trapezoidcpy(currentId);
    bool isQ1EqualTopP2 = geoutils::arePointsEqual(segment.p2(), current.top().p2());
    bool isQ1EqualBotP2 = geoutils::arePointsEqual(segment.p2(), current.bottom().p2());

    cg3::Point2d p1 = cg3::Point2d(current.leftp().x(), geoutils::calculateYCoord(segment, current.leftp().x()));
    cg3::Segment2d innerSegment = cg3::Segment2d(p1, segment.p2());

    std::vector<Trapezoid> hSplit = std::vector<Trapezoid>();
    tMap.SplitHorizontaly(current, innerSegment, hSplit);

    lastTrapezoidsInserted.clear();
    lastTrapezoidsInserted.push_back(tMap.replace(currentId,hSplit[0]));
    lastTrapezoidsInserted.push_back(tMap.addTrapezoid(hSplit[1]));

    size_t idUpper = lastTrapezoidsInserted[0];
    size_t idLower = lastTrapezoidsInserted[1];
    Trapezoid& referenceLower = tMap.trapezoid(idLower);
    Trapezoid& referenceUpper = tMap.trapezoid(idUpper);

    Trapezoid mergeCandidate = tMap.trapezoid(tMap.getIdElegibleForMerge());
    if(tMap.canTheyMerge(mergeCandidate, referenceLower)){
        referenceUpper.updateLeftNeighborsOld(current);
        tMap.addElegibleForMerge(idLower);
        lastTrapezoidsInserted[1] = tMap.PerformeMerge();
        Trapezoid& referenceLower = tMap.trapezoid(lastTrapezoidsInserted[1]);
        tMap.indirectUpdateNeighbors(current, currentId, true, idUpper);

        if (isQ1EqualTopP2 and !isQ1EqualBotP2){
            referenceUpper.updateRightNeighbors(SIZE_MAX);
            referenceLower.updateRightNeighborsOld(current);
            tMap.indirectUpdateNeighbors(current, currentId, false, lastTrapezoidsInserted[1]);
        }else if(!isQ1EqualTopP2 and isQ1EqualBotP2){
            referenceLower.updateRightNeighbors(SIZE_MAX);
            referenceUpper.updateRightNeighborsOld(current);
            tMap.indirectUpdateNeighbors(current, currentId, false, lastTrapezoidsInserted[0]);
        }else if(isQ1EqualTopP2 and isQ1EqualBotP2){
            referenceLower.updateRightNeighbors(SIZE_MAX);
            referenceUpper.updateRightNeighbors(SIZE_MAX);
            if(current.upperLeftNeighbor() == current.lowerLeftNeighbor()){
                referenceUpper.updateLeftNeighbors(current.upperLeftNeighbor());
            }else{
                referenceUpper.updateLeftNeighborsOld(current);
            }
        }else{
            referenceUpper.updateLeftNeighborsOld(current);
            referenceUpper.updateRightNeighbors(current.upperRightNeighbor());
            referenceLower.updateRightNeighbors(current.lowerRightNeighbor());
            tMap.indirectUpdateNeighbors(current, currentId, true, idUpper);
            tMap.trapezoid(current.upperRightNeighbor()).indirectUpdateNeighborsLeft(currentId, idUpper);
            tMap.trapezoid(current.lowerRightNeighbor()).indirectUpdateNeighborsLeft(currentId, lastTrapezoidsInserted[1]);
        }
    }else{
        referenceLower.updateLeftNeighborsOld(current);
        tMap.addElegibleForMerge(idUpper);
        lastTrapezoidsInserted[0] = tMap.PerformeMerge();
        Trapezoid& referenceUpper = tMap.trapezoid(lastTrapezoidsInserted[0]);
        tMap.indirectUpdateNeighbors(current, currentId, true, idLower);
        if (isQ1EqualTopP2 and !isQ1EqualBotP2){
            referenceUpper.updateRightNeighbors(SIZE_MAX);
            referenceLower.updateRightNeighborsOld(current);
            tMap.indirectUpdateNeighbors(current, currentId, false, lastTrapezoidsInserted[1]);
        }else if(!isQ1EqualTopP2 and isQ1EqualBotP2){
            referenceLower.updateRightNeighbors(SIZE_MAX);
            referenceUpper.updateRightNeighborsOld(current);
            tMap.indirectUpdateNeighbors(current, currentId, false, lastTrapezoidsInserted[0]);
        }else if(isQ1EqualTopP2 and isQ1EqualBotP2){
            referenceLower.updateRightNeighbors(SIZE_MAX);
            referenceUpper.updateRightNeighbors(SIZE_MAX);
            if(current.upperLeftNeighbor() == current.lowerLeftNeighbor()){
                referenceLower.updateLeftNeighbors(current.lowerLeftNeighbor());
            }else{
                referenceLower.updateLeftNeighborsOld(current);
            }
        }else{
            referenceLower.updateLeftNeighborsOld(current);
            referenceLower.updateRightNeighbors(current.lowerRightNeighbor());
            referenceUpper.updateRightNeighbors(current.upperRightNeighbor());
            tMap.indirectUpdateNeighbors(current, currentId, true, idLower);
            tMap.trapezoid(current.upperRightNeighbor()).indirectUpdateNeighborsLeft(currentId, lastTrapezoidsInserted[0]);
            tMap.trapezoid(current.lowerRightNeighbor()).indirectUpdateNeighborsLeft(currentId, idLower);
        }
    }

}
/**
 * @brief Handle the case when the p1 is inside the trapezoid and the p1 is out of the trapezoid.
 * Split the trapezoid in three other trapezoids, push their id in the lastTrapezoidInserted vector. Based on the fact that the rightp is
 * above or below the segment push the lower or the upper trapezoid in the elegibleformerge vector. Updates the adjacency for the next
 * step.
 * @param currentId, id of the trapezoid to split in the map.
 * @param segment, segment which is interesecting the current trapezoid.
 * @param lastTrapezoidsInserted, vector that will store the ids of the newly added segment in order to insert them in the dag.
 * @return The two trapezoid obtained by the split with the adjacency updated, pushed into the vector lastTrapezoidsInserted.
 */
void handleCaseP1Inside(TrapezoidalMap& tMap, const size_t& currentId, const cg3::Segment2d& segment, std::vector<size_t>& lastTrapezoidsInserted){
    Trapezoid current = tMap.trapezoidcpy(currentId);
    bool isPointAbove = geoutils::isPointAbove(current.rightp(), segment);
    tMap.SplitInThree(currentId, segment, segment.p1(), lastTrapezoidsInserted);

    /*Add new trapezoid on the map*/
    size_t idLeft = lastTrapezoidsInserted[0];
    size_t idRightUpper = lastTrapezoidsInserted[1];
    size_t idRightLower = lastTrapezoidsInserted[2];

    Trapezoid& referenceLeft = tMap.trapezoid(idLeft);
    Trapezoid& referenceRightU = tMap.trapezoid(idRightUpper);
    Trapezoid& referenceRightL = tMap.trapezoid(idRightLower);

    /*Set neighbors internally for firstTSplit*/
    referenceLeft.updateLeftNeighborsOld(current);
    referenceLeft.updateRightNeighbors(idRightUpper, idRightLower);
    referenceRightU.updateLeftNeighbors(idLeft);
    referenceRightL.updateLeftNeighbors(idLeft);
    referenceRightU.updateRightNeighborsOld(current);
    referenceRightL.updateRightNeighborsOld(current);
    tMap.indirectUpdateNeighbors(current, currentId, true, idLeft);

    if (isPointAbove){
        tMap.addElegibleForMerge(idRightLower);
        tMap.indirectUpdateNeighbors(current, currentId, false, idRightUpper);
        /*Merge upperr*/
    }else{
        tMap.addElegibleForMerge(idRightUpper);
        tMap.indirectUpdateNeighbors(current, currentId, false, idRightLower);
    }

}
/**
 * @brief Handle the case when the p1 is outside and the q1 is outside.
 * Split the trapezoid in two other trapezoids, push their id in the lastTrapezoidInserted vector. Based on the fact that the rightp is
 * above or below the segment push the lower or the upper trapezoid in the elegibleformerge vector. Uses the lastTrapezoidsIntersected
 * of the previous step for update the adjacency, for the next step. Then push the new created trapezoids in the lastTrapezoidsInserted.
 * @param currentId, id of the trapezoid to split in the map.
 * @param segment, segment which is interesecting the current trapezoid.
 * @param lastTrapezoidsInserted, vector that will store the ids of the newly added segment in order to insert them in the dag.
 * @return The two trapezoid obtained by the split with the adjacency updated, pushed into the vector lastTrapezoidsInserted.
 */
void handleCasePointsOutside(TrapezoidalMap& tMap, const size_t& currentId, const cg3::Segment2d& segment, std::vector<size_t>& lastTrapezoidsInserted){
    Trapezoid current = tMap.trapezoidcpy(currentId);

    bool isPointAbove = geoutils::isPointAbove(current.rightp(), segment);
    cg3::Point2d p1 = cg3::Point2d(current.leftp().x(), geoutils::calculateYCoord(segment, current.leftp().x()));
    cg3::Point2d q1 = cg3::Point2d(current.rightp().x(), geoutils::calculateYCoord(segment, current.rightp().x()));
    cg3::Segment2d innerSegment = cg3::Segment2d(p1, q1);
    std::vector<Trapezoid> hSplit = std::vector<Trapezoid>();
    tMap.SplitHorizontaly(current, innerSegment, hSplit);

    size_t idUpper = tMap.replace(currentId, hSplit[0]);
    size_t idLower = tMap.addTrapezoid(hSplit[1]);

    Trapezoid& referenceUpper = tMap.trapezoid(idUpper);
    Trapezoid& referenceLower = tMap.trapezoid(idLower);

    /*Merge handling*/
    referenceUpper.updateRightNeighborsOld(current);
    referenceLower.updateRightNeighborsOld(current);
    Trapezoid& mergeCandidate = tMap.trapezoid(tMap.getIdElegibleForMerge());
    size_t merged = tMap.getIdElegibleForMerge();
    /*Merge with lower*/
    if(tMap.canTheyMerge(mergeCandidate, referenceLower)){
        tMap.addElegibleForMerge(idLower);
        idLower = tMap.PerformeMerge();
        size_t leftNeighbor = lastTrapezoidsInserted.size() == 3 ? lastTrapezoidsInserted[1]:lastTrapezoidsInserted[0];
        referenceUpper.updateLeftNeighborsOld(current);
        tMap.trapezoid(leftNeighbor).updateLRNeighbor(idUpper);
        /*Merge isn't finished yet*/
        if (isPointAbove){
            tMap.addElegibleForMerge(idLower);
            tMap.indirectUpdateNeighbors(current, currentId, false, idUpper);
            /*Merge is concluded*/
        }else{
            tMap.addElegibleForMerge(idUpper);
            tMap.indirectUpdateNeighbors(current, currentId, false, merged);
        }
        tMap.indirectUpdateNeighbors(current, currentId, true, idUpper);
    }else{
        tMap.addElegibleForMerge(idUpper);
        idUpper = tMap.PerformeMerge();
        size_t leftNeighbor = lastTrapezoidsInserted.size() == 3 ? lastTrapezoidsInserted[2]:leftNeighbor = lastTrapezoidsInserted[1];

        referenceLower.updateLeftNeighborsOld(current);
        tMap.trapezoid(leftNeighbor).updateURNeighbor(idLower);
        if (isPointAbove){
            tMap.addElegibleForMerge(idLower);
            tMap.indirectUpdateNeighbors(current,currentId, false, merged);
            /*Merge is concluded*/
        }else{
            tMap.addElegibleForMerge(idUpper);
            tMap.indirectUpdateNeighbors(current, currentId, false, idLower);
        }
        tMap.indirectUpdateNeighbors(current, currentId, true, idLower);
    }

    lastTrapezoidsInserted.clear();
    lastTrapezoidsInserted.push_back(idUpper);
    lastTrapezoidsInserted.push_back(idLower);
}
/**
 * @brief Handle the case when the p1 is outside and the q1 is inside the current trapezoid.
 * Split the trapezoid in three other trapezoids, push their id in the lastTrapezoidInserted vector. Based on the fact that the rightp is
 * above or below the segment push the lower or the upper trapezoid in the elegibleformerge vector. Uses the lastTrapezoidsIntersected
 * of the previous step for update the adjacency, for the next step. Then push the new created trapezoids in the lastTrapezoidsInserted.
 * @param currentId, id of the trapezoid to split in the map.
 * @param segment, segment which is interesecting the current trapezoid.
 * @param lastTrapezoidsInserted, vector that will store the ids of the newly added segment in order to insert them in the dag.
 * @return The two trapezoid obtained by the split with the adjacency updated, pushed into the vector lastTrapezoidsInserted.
 */
void handleCaseQ1Inside(TrapezoidalMap& tMap, const size_t& currentId, const cg3::Segment2d& segment, std::vector<size_t>& lastTrapezoidsInserted){
    Trapezoid current = tMap.trapezoidcpy(currentId);

    lastTrapezoidsInserted.clear();
    tMap.SplitInThree(currentId, segment, segment.p2(), lastTrapezoidsInserted);

    /*Add new trapezoid on the map*/
    size_t idRight = lastTrapezoidsInserted[0];
    size_t idLeftUpper = lastTrapezoidsInserted[1];
    size_t idLeftLower = lastTrapezoidsInserted[2];

    Trapezoid& referenceRight = tMap.trapezoid(idRight);
    Trapezoid& referenceLeftU = tMap.trapezoid(idLeftUpper);
    Trapezoid& referenceLeftL = tMap.trapezoid(idLeftLower);

    /*Set neighbors internally for tSplitResult*/
    referenceRight.updateRightNeighborsOld(current);
    referenceRight.updateLeftNeighbors(idLeftUpper, idLeftLower);
    referenceLeftU.updateRightNeighbors(idRight);
    referenceLeftL.updateRightNeighbors(idRight);
    referenceLeftL.updateLeftNeighborsOld(current);
    referenceLeftU.updateLeftNeighborsOld(current);
    tMap.indirectUpdateNeighbors(current, currentId, false, idRight);

    Trapezoid& mergeCandidate = tMap.trapezoid(tMap.getIdElegibleForMerge());
    if(tMap.canTheyMerge(mergeCandidate, referenceLeftL)){
        tMap.addElegibleForMerge(idLeftLower);
        size_t mergeResult = tMap.PerformeMerge();
        lastTrapezoidsInserted[2] = mergeResult;
        referenceRight.updateLLNeighbor(mergeResult);
        tMap.indirectUpdateNeighbors(current, currentId, true, idLeftUpper);
    }else{
        tMap.addElegibleForMerge(idLeftUpper);
        size_t mergeResult = tMap.PerformeMerge();
        lastTrapezoidsInserted[1] = mergeResult;
        referenceRight.updateULNeighbor(mergeResult);
        tMap.indirectUpdateNeighbors(current, currentId, true, idLeftLower);

    }
}



/**
 * \brief Creates a subgraph depending by the case and insert it in place of the current trapezoid node.
 * Performe the update of the dag, generating the subgraph correspondent to the case and substituing the root of the subgraph
 * in place of the current trapezoid intersected. Using the same id the fathers node of the removed one stil point to the correct
 * node without need of performing extra operations.
 * @param tMap
 * @param dag
 * @param current, the trapezoid intersected by the segment that needs to be removed from the dag, the remove is done in one step.
 * @param s, the segment inserted in the trapezoidal map.
 * @param nodes, a vector that holds the nodes of the segment and his endpoints. (The order is: P1, S1, Q1)
 * @param idsTrapezoid, the ids of the new inserted trapezoids, will be used to create for each one of them a leaf node
 * and set their crosslink.
 * @param degenerative, boolean used to handle the degenerative cases. Handle the update of the dag when one of the segment's
 * endpoint is equal to rightp or leftp of the intersected trapezoid.
 * @return Dag updated with the new trapezoids and current removed from dag.
 */
void performeDagUpdate(TrapezoidalMap& tMap, Dag& dag, const Trapezoid& current, const cg3::Segment2d s, const std::vector<Node>& nodes,  std::vector<size_t>& idsTrapezoid, bool degenerative){
    /*Degenerate case*/
    if(degenerative){
        size_t idP1 = dag.replace(current.node(), nodes[0]);
        size_t idS1 = dag.addNode(nodes[1]);
        size_t idQ1 = dag.addNode(nodes[2]);

        /*LeafNode generation and update of crosslink to the node*/
        for(size_t i=0; i < idsTrapezoid.size(); i++){
            tMap.trapezoid(idsTrapezoid[i]).setNode(dag.addNode(Node(leafNode, idsTrapezoid[i]))); //sets the double link between node and trapezoid while adding the new node in the dag
        }

        if ((geoutils::arePointsEqual(s.p1(),current.top().p1()) or geoutils::arePointsEqual(s.p1(),current.bottom().p1()) or geoutils::arePointsEqual(s.p1(), current.leftp())) and s.p2().x() < current.rightp().x()){
            dag.addChildrenToNode(idP1, SIZE_MAX, idQ1);
            dag.addChildrenToNode(idQ1, idS1, tMap.trapezoid(idsTrapezoid[0]).node());
            dag.addChildrenToNode(idS1, tMap.trapezoid(idsTrapezoid[1]).node(), tMap.trapezoid(idsTrapezoid[2]).node());
        }else{
            dag.addChildrenToNode(idP1, tMap.trapezoid(idsTrapezoid[0]).node(), idQ1);
            dag.addChildrenToNode(idQ1, idS1, SIZE_MAX);
            dag.addChildrenToNode(idS1, tMap.trapezoid(idsTrapezoid[1]).node(), tMap.trapezoid(idsTrapezoid[2]).node());
        }

    }
    /*Case p1 and q1 inside trapezoid*/
    else if(s.p1() >= current.leftp() and s.p2() <= current.rightp()){
        size_t idP1 = dag.replace(current.node(), nodes[0]);
        size_t idS1 = dag.addNode(nodes[1]);
        size_t idQ1 = dag.addNode(nodes[2]);

        /*LeafNode generation and update of trapezoid pointer to the node*/
        for(size_t i=0; i < idsTrapezoid.size(); i++){
            tMap.trapezoid(idsTrapezoid[i]).setNode(dag.addNode(Node(leafNode, idsTrapezoid[i]))); //sets the double link between node and trapezoid while adding the new node in the dag
        }
        dag.addChildrenToNode(idP1, tMap.trapezoid(idsTrapezoid[0]).node(), idQ1);
        dag.addChildrenToNode(idQ1, idS1, tMap.trapezoid(idsTrapezoid[3]).node());
        dag.addChildrenToNode(idS1, tMap.trapezoid(idsTrapezoid[1]).node(), tMap.trapezoid(idsTrapezoid[2]).node());

    }else if (s.p1() >= current.leftp() and s.p2() > current.rightp()){ //Case p1 inside trapezoid
        size_t idP1 = dag.replace(current.node(), nodes[0]);
        size_t idS1 = dag.addNode(nodes[1]);
        /*LeafNode generation and update of trapezoid pointer to the node*/
        for(size_t i=0; i < idsTrapezoid.size(); i++){
            tMap.trapezoid(idsTrapezoid[i]).setNode(dag.addNode(Node(leafNode, idsTrapezoid[i]))); //sets the double link between node and trapezoid while adding the new node in the dag
        }
        if(idsTrapezoid.size() == 2){
            dag.addChildrenToNode(idP1, SIZE_MAX, idS1);
            dag.addChildrenToNode(idS1, tMap.trapezoid(idsTrapezoid[0]).node(), tMap.trapezoid(idsTrapezoid[1]).node());
        }else{
            dag.addChildrenToNode(idP1, tMap.trapezoid(idsTrapezoid[0]).node(), idS1);
            dag.addChildrenToNode(idS1, tMap.trapezoid(idsTrapezoid[1]).node(), tMap.trapezoid(idsTrapezoid[2]).node());
        }

        /*Case p1 and q1 outside trapezoid*/
    }else if(s.p1() < current.leftp() and s.p2() > current.rightp()){
        size_t idS1 = dag.replace(current.node(), nodes[1]);
        /*LeafNode generation and update of trapezoid pointer to the node*/
        for(size_t i=0; i < idsTrapezoid.size(); i++){
            if (tMap.trapezoid(idsTrapezoid[i]).node() == SIZE_MAX){
                tMap.trapezoid(idsTrapezoid[i]).setNode(dag.addNode(Node(leafNode, idsTrapezoid[i]))); //sets the double link between node and trapezoid while adding the new node in the dag
            }
        }
        dag.addChildrenToNode(idS1, tMap.trapezoid(idsTrapezoid[0]).node(), tMap.trapezoid(idsTrapezoid[1]).node());
        /*Case q1 only inside trapezoid*/
    }else if(s.p1() < current.leftp() and s.p2() <= current.rightp()){
        size_t idQ1 = dag.replace(current.node(), nodes[2]);
        size_t idS1 = dag.addNode(nodes[1]);

        /*LeafNode generation and update of trapezoid pointer to the node*/
        for(size_t i=0; i < idsTrapezoid.size(); i++){
            if (tMap.trapezoid(idsTrapezoid[i]).node() == SIZE_MAX){
                tMap.trapezoid(idsTrapezoid[i]).setNode(dag.addNode(Node(leafNode, idsTrapezoid[i]))); //sets the double link between node and trapezoid while adding the new node in the dag
            }
        }
        if(idsTrapezoid.size() == 2){
            dag.addChildrenToNode(idS1, tMap.trapezoid(idsTrapezoid[0]).node(), tMap.trapezoid(idsTrapezoid[1]).node());
            dag.addChildrenToNode(idQ1, idS1, SIZE_MAX);
        }else{
            dag.addChildrenToNode(idS1, tMap.trapezoid(idsTrapezoid[1]).node(), tMap.trapezoid(idsTrapezoid[2]).node());
            dag.addChildrenToNode(idQ1, idS1, tMap.trapezoid(idsTrapezoid[0]).node());
        }
    }

}


/**
 * @brief queryPoint performe the point location of a given point p1 returning the id of the trapezoid where the point is located.
 * Using the searching structure dag, search for a point p1 until a leafNode is found. If the current node is a xNode checks
 * his x comparing it with the point to decide the turn, if it is a yNode checks if the point is above or below the segment, in case
 * the point and the segment are collinear (Degenerate case) uses the second point q1 to decide the turn. When a leafNode is found
 * return his id.
 * @note In case of single point location p1 == q1
 * @param tm
 * @param dag
 * @param p1, point to query
 * @param q1, second point to query in case the first point is equal to another point already in the map.
 * @return The id of the trapezoid where the point is located.
 */
size_t queryPoint(const TrapezoidalMap& tm, const Dag& dag,const cg3::Point2d& p1,const cg3::Point2d& q1){
    Node tmp = dag.root();
    while(tmp.type() != leafNode){
        if(tmp.type() == xNode){
            if (p1.x() < tm.point(tmp.id()).x()){
                tmp = dag.node(tmp.left());
            }else{
                tmp = dag.node(tmp.right());
            }
        }
        if(tmp.type() == yNode){
            if(geoutils::isPointAbove(p1, tm.segment(tmp.id())) > 0){ //check if a poin is above a segment function
                tmp = dag.node(tmp.left());
            }else if(geoutils::isPointAbove(p1, tm.segment(tmp.id())) == -1){
                if(geoutils::isPointAbove(q1, tm.segment(tmp.id()))){
                    tmp = dag.node(tmp.left());
                }else{
                    tmp = dag.node(tmp.right());
                }
            }else{
                tmp = dag.node(tmp.right());
            }
        }

    }
    return tmp.id();
};


/**
 * @brief followSegment, return a list of trapezoid intersected by the segment s.
 * The first trapezoid is obtained using the point location function query point, from it the others are obtained looking to
 * his right neighbors.
 * @param tMap
 * @param dag
 * @param s, segment that is getting inserted in the trapezoidalMap.
 * @return fill the list trapezoidsIntersected with the trapezoids intersected by the segment s.
 */
void followSegment(const TrapezoidalMap& tMap, const Dag& dag, const cg3::Segment2d& s, std::vector<size_t>& trapezoidsIntersected){
    trapezoidsIntersected.push_back(queryPoint(tMap, dag, s.p1(), s.p2()));
    int j = 0;
    while (s.p2().x() > tMap.trapezoid(trapezoidsIntersected[j]).rightp().x()){
        if (geoutils::isPointAbove(tMap.trapezoid(trapezoidsIntersected[j]).rightp(), s)){
            trapezoidsIntersected.push_back(tMap.trapezoid(trapezoidsIntersected[j]).lowerRightNeighbor());
        }else{
            trapezoidsIntersected.push_back(tMap.trapezoid(trapezoidsIntersected[j]).upperRightNeighbor());
        }
        j++;
    }
}

}
