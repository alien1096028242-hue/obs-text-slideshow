#pragma once
#include <obs-frontend-api.h>
#include <QDockWidget>
#include <QComboBox>
#include <QListWidget>
#include <memory>
#include <vector>
#include "ui_obs-text-slideshow-dock.h"

using std::vector;

struct slideshow_t {
    obs_source_t *source;
    int index;
};

class TextSlideshowDock : public QDockWidget {
    Q_OBJECT
private:
    static void OBSFrontendEventWrapper(enum obs_frontend_event event, void *ptr);
    void OBSFrontendEvent(enum obs_frontend_event event);
    void changeActivePreviewSource(int index);
    void changeActiveProgramSource(int index);
    void setActiveSource(int index, QComboBox *sourceBox, vector<obs_source_t *> &text_slideshows, struct slideshow_t *active_slideshow);
    void chooseNewActiveSource(QComboBox *sourceBox, vector<obs_source_t *> &text_slideshows, struct slideshow_t *active_slideshow);
    void updateSources(obs_source_t *scene_source, QComboBox *sourceBox, vector<obs_source_t *> &text_slideshows, struct slideshow_t *active_slideshow);
    void updateTexts(QListWidget *textList, vector<const char *> &texts, struct slideshow_t *active_slideshow);
    void previewTransition(QListWidgetItem *item);
    void programTransition(QListWidgetItem *item);

    // --- 以下是之前可能丢失的关键成员变量 ---
    std::unique_ptr<Ui::TextSlideshowDock> ui;
    vector<obs_source_t *> preview_text_slideshows;
    vector<const char *> preview_texts;
    struct slideshow_t preview_active_slideshow;
    vector<obs_source_t *> program_text_slideshows;
    vector<const char *> program_texts;
    struct slideshow_t program_active_slideshow;

public:
    TextSlideshowDock(QWidget *parent = nullptr);
    ~TextSlideshowDock(); // 声明析构函数
    void refreshPreview();
    void refreshProgram();
    virtual void dock_get_texts(const char *texts); // OBS 30+ 必须的虚函数
};
