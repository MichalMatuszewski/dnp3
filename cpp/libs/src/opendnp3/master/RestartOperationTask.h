/*
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#ifndef OPENDNP3_RESTART_OPERATION_TASK_H
#define OPENDNP3_RESTART_OPERATION_TASK_H

#include "opendnp3/master/IMasterTask.h"
#include "opendnp3/master/RestartOperationResult.h"
#include "opendnp3/master/TaskPriority.h"
#include "opendnp3/app/parsing/IAPDUHandler.h"

#include "opendnp3/gen/RestartType.h"

namespace opendnp3
{

class RestartOperationTask final : public IMasterTask, private IAPDUHandler
{

public:

	RestartOperationTask(IMasterApplication& app, const openpal::MonotonicTimestamp& startTimeout, RestartType operationType, const RestartOperationCallbackT& callback, openpal::Logger logger, const TaskConfig& config);

	virtual bool BuildRequest(APDURequest& request, uint8_t seq) override;

	virtual int Priority() const
	{
		return priority::USER_REQUEST;
	}

	virtual bool BlocksLowerPriority() const
	{
		return false;
	}

	virtual bool IsRecurring() const
	{
		return false;
	}

	virtual char const* Name() const override;

	virtual bool IsAllowed(uint32_t headerCount, GroupVariation gv, QualifierCode qc) override;

private:

	virtual MasterTaskType GetTaskType() const override;

	virtual IINField ProcessHeader(const CountHeader& header, const ICollection<Group52Var1>& values) override;
	virtual IINField ProcessHeader(const CountHeader& header, const ICollection<Group52Var2>& values) override;

	static FunctionCode ToFunctionCode(RestartType op);

	const FunctionCode function;
	RestartOperationCallbackT callback;
	openpal::TimeDuration duration = openpal::TimeDuration::Min();


	virtual IMasterTask::ResponseResult ProcessResponse(const opendnp3::APDUResponseHeader& header, const openpal::RSlice& objects) override;

	virtual void OnTaskComplete(TaskCompletion result, openpal::MonotonicTimestamp now) override;
};

} //end ns


#endif
