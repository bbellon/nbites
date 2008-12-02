#include "ScriptedProvider.h"
#include "Kinematics.h"



using namespace Kinematics;


ScriptedProvider::ScriptedProvider(float motionFrameLength,
								   Sensors *s)
	: MotionProvider(),
	  FRAME_LENGTH_S(motionFrameLength),
	  chopper(s, FRAME_LENGTH_S),
	  nextJoints(),
	  choppedCommand(0),
	  headQueue(HEAD_CHAIN),
	  lArmQueue(LARM_CHAIN),
	  lLegQueue(LLEG_CHAIN),
	  rLegQueue(RLEG_CHAIN),
	  rArmQueue(RARM_CHAIN)
{


	// Create mutexes
	// (?) Need one mutex per queue (?)
	pthread_mutex_init (&scripted_mutex, NULL);
}

ScriptedProvider::~ScriptedProvider() {
	// remove all remaining values from chain queues
}

void ScriptedProvider::requestStop() {
	// Finish motion or stop immediately?

}

void ScriptedProvider::calculateNextJoints() {
	// Sets the next joints for all the joints
	setNextChainJoints( LARM_CHAIN, lArmQueue.front() );
	setNextChainJoints( LLEG_CHAIN, lLegQueue.front() );
	setNextChainJoints( RLEG_CHAIN, rLegQueue.front() );
	setNextChainJoints( RARM_CHAIN, rArmQueue.front() );

	lArmQueue.pop();
	lLegQueue.pop();
	rLegQueue.pop();
	rArmQueue.pop();

}

void ScriptedProvider::enqueue(const BodyJointCommand *command) {
	// Chop command
	choppedCommand = chopper.chopCommand(command);

	//Split command by chops
	vector<vector<vector<float> > >::iterator choppedIter;
	choppedIter = choppedCommand.begin();
	for ( ; choppedIter != choppedCommand.end() ; choppedIter++){

		// Pass each chain to its chainqueue
		headQueue.push(choppedIter[HEAD_CHAIN]);
		lArmQueue.push(choppedIter[LARM_CHAIN]);
		lLegQueue.push(choppedIter[LLEG_CHAIN]);
		rLegQueue.push(choppedIter[RLEG_CHAIN]);
		rArmQueue.push(choppedIter[RARM_CHAIN]);
	}

}

}

void ScriptedProvider::enqueueSequence(std::vector<BodyJointCommand*> &seq) {
	// Take in vec of commands and enqueue them all
	pthread_mutex_lock(&scripted_mutex);
	for (vector<BodyJointCommand*>::iterator i= seq.begin(); i != seq.end(); i++)
		enqueue(*i);
	pthread_mutex_unlock(&scripted_mutex);

}
