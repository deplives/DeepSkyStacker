#ifndef __BATCHSTACKING_H__
#define __BATCHSTACKING_H__

#include <QDialog>

class QStandardItemModel;

namespace Ui {
	class BatchStacking;
}

namespace DSS
{
	//TODO: move this in its own file
	class BaseDialog : public QDialog
	{
		Q_OBJECT

			typedef QDialog
			Inherited;
	public:
		enum class Behaviour
		{
			None				= 0x00,
			PersistGeometry		= 0x01
		};
		Q_DECLARE_FLAGS(Behaviours, Behaviour)
	public:
		BaseDialog(const Behaviours& behaviours = Behaviour::None, QWidget* parent = nullptr);

	protected:
		void showEvent(QShowEvent* event) override;

	private slots:
		void saveState() const;

	private:
		virtual void onInitDialog();
		bool hasPersistentGeometry() const {
			return m_behaviours.testFlag(Behaviour::PersistGeometry);
		}

	private:
		Behaviours		m_behaviours{ Behaviour::None };
		bool			m_initialised{ false };

	private:
		Q_DISABLE_COPY(BaseDialog)
	};

	Q_DECLARE_OPERATORS_FOR_FLAGS(BaseDialog::Behaviours)

	class BatchStacking : public BaseDialog
	{
		Q_OBJECT

			typedef BaseDialog
			Inherited;

	public:
		BatchStacking(QWidget* parent = nullptr);
		virtual ~BatchStacking();

		void setMRUPaths(const std::vector<fs::path>& mruPaths);

	public slots:
		void accept() override;
		void clearLists();
		void addLists();

	private:
		void addItemsFor(const QStringList& paths, bool checked);
		QStringList getFilePaths() const;

	private:
		Ui::BatchStacking*	ui{ nullptr };
		QStandardItemModel*	m_fileListModel{ nullptr };

	private:
		Q_DISABLE_COPY(BatchStacking)
	};
}


#endif