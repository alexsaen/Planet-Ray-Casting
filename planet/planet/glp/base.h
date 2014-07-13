/*
Copyright (c) 2014, Alexey Saenko
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef GLP_BASE_H
#define GLP_BASE_H

namespace glp {

	template<typename T, void (*Release)(T)>
	class ID {
	public:
		ID(): mID(0), mCounter(0) {}

		ID(T mid): mID(mid), mCounter(0) {}

		~ID() {
			decRef();			
		}

		ID(const ID& i) {
			copy(i);
		}

		ID& operator=(const ID& i) {
			decRef();
			copy(i);
			return *this;
		}

		T& getID() {
			return mID;
		}

		operator T() const {
			return mID;
		}

	protected:

		T mID;
		mutable int	*mCounter;

		void copy(const ID& i) {
			mID = i.mID;
			if(i.mCounter) {
				mCounter = i.mCounter;
				(*mCounter)++;
			} else {
				mCounter = i.mCounter = new int;
				*mCounter = 2;
			}
		}

		void decRef() {

			if(!mCounter) {
				if(mID)
					Release(mID);
				return;
			}

			(*mCounter)--;
			if((*mCounter) == 0) {
				delete mCounter;
				if(mID)
					Release(mID);
			}

		}

	};


}

#endif

