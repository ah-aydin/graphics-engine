#pragma once

namespace kbb
{
	class GameObject
	{
	protected:
		using id_t = unsigned int;

		template<class C>
		static C createGameObject()
		{
			static id_t currentId = 0;
			return C{ currentId++ };
		};

		GameObject(id_t objId) : m_id(objId) {}

		GameObject(const GameObject&) = delete;
		GameObject& operator=(const GameObject&) = delete;
		GameObject(GameObject&&) = default;
		GameObject& operator=(GameObject&&) = default;

		id_t m_id;

	public:
		id_t getId() { return m_id; }
	};
}
