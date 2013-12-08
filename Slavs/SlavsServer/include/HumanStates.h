#ifndef HumanStates_h
#define HumanStates_h

#include <Patterns/State.h>

class SHumanComponent;

class HumanGlobal : public State<SHumanComponent, long>
{
public:
	HumanGlobal();
	~HumanGlobal();

	virtual void Enter(std::shared_ptr<SHumanComponent> ip_owner) override;
	virtual void Execute(std::shared_ptr<SHumanComponent> ip_owner, long i_elapsed_time) override;
	virtual void Exit(std::shared_ptr<SHumanComponent> ip_owner) override;

	virtual bool OnMessage(std::shared_ptr<SHumanComponent> ip_owner, const Telegram& i_message) override;
};

class HumanIdle : public State<SHumanComponent, long>
{
public:
	HumanIdle();
	~HumanIdle();

  virtual void Enter(std::shared_ptr<SHumanComponent> ip_owner) override;
  virtual void Execute(std::shared_ptr<SHumanComponent> ip_owner, long i_elapsed_time) override;
  virtual void Exit(std::shared_ptr<SHumanComponent> ip_owner) override;

  virtual bool OnMessage(std::shared_ptr<SHumanComponent> ip_owner, const Telegram& i_message) override;
};

class HumanMove : public State<SHumanComponent, long>
{
public:
	HumanMove();
	~HumanMove();

  virtual void Enter(std::shared_ptr<SHumanComponent> ip_owner) override;
  virtual void Execute(std::shared_ptr<SHumanComponent> ip_owner, long i_elapsed_time) override;
  virtual void Exit(std::shared_ptr<SHumanComponent> ip_owner) override;

  virtual bool OnMessage(std::shared_ptr<SHumanComponent> ip_owner, const Telegram& i_message) override;
};

class HumanWork : public State<SHumanComponent, long>
{
public:
	HumanWork();
	~HumanWork();

  virtual void Enter(std::shared_ptr<SHumanComponent> ip_owner) override;
  virtual void Execute(std::shared_ptr<SHumanComponent> ip_owner, long i_elapsed_time) override;
  virtual void Exit(std::shared_ptr<SHumanComponent> ip_owner) override;

  virtual bool OnMessage(std::shared_ptr<SHumanComponent> ip_owner, const Telegram& i_message) override;
};
#endif