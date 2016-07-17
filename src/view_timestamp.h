/*
 * A Window that displays a timestamp,
 * any associated notes input from the phone app,
 * and an ActionBarLayer to allow the user to
 * delete the timestamp, and possibly other
 * actions in the future.
 */

// void show_view_timestamp(void);
void hide_view_timestamp(void);

/*
 * Callback to start the view_timestamp window.
 * @param idx: The index in the storage array of the selected timestamp.
 * @param ctx: The context parameter of the original Layer.
 */
void view_timestamp(int idx, void *ctx);