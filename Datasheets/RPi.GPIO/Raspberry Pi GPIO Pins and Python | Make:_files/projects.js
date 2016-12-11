jQuery(document).ready(function ($) {

    // Handle the click actions on the list items in the steps box
    $('body').on('click', '#tabs li.steps', function () {
        var id = $(this).attr('id');

        // Progress the slider
        $('#steppers').find('.jstep#js-' + id).slideDown().removeClass('hide');
        $('#steppers').find('.jstep:not( #js-' + id + ')').slideUp({ 
            complete: function(){
                $('.jstep#js-' + id + ' img.lazyload').lazyload();
            }
        });


        // Update the side navigation list
        $(this).addClass('current');
        $('#tabs li:not(#' + id + ')').removeClass('current');

        // Run our trackers
        googletag.pubads().refresh();
        ga('send', 'pageview');
    });

    // Allows us to advance in the slider
    $('body').on('click', '.nexter', function () {
        var id = $(this).attr('id');

        // Progress the slider
        $('#steppers').find('.jstep#js-' + id).slideDown().removeClass('hide');
        $('#steppers').find('.jstep:not( #js-' + id + ')').slideUp({ 
            complete: function(){
                $('.jstep#js-' + id + ' img.lazyload').lazyload();
            }
        });

        // Update side navigation list
        $('#tabs').find(' li#' + id).addClass('current');
        $('#tabs').find('li:not( #' + id + ')').removeClass('current');

        // Run our trackers
        googletag.pubads().refresh();
        ga('send', 'pageview');
    });

    // Display all projects steps when we click "View All"
    $('body').on('click', '.aller', function () {
        // Display all the slides
        $('#steppers').find('.jstep').each(function () {
            $(this).removeClass('hide');
            $(this).slideDown();
        });

        // Hide the next/previous buttons
        $('#steppers .nexter, #steppers .disabled').hide();

        // Run our trackers
        googletag.pubads().refresh();
        ga('send', 'pageview');
    });

    jQuery('.carousel').on('slide', function () {
        jQuery('.slide').find('iframe').each(function () {
            jQuery(this).attr('src', '');
            var url = jQuery(this).attr('data-src');
            jQuery(this).delay(1000).attr('src', url);
        });
        // Find the active slide, and then add an active class to the thumb.
        var index = jQuery(this).find('.active').data('index');
        jQuery('.inner-thumbs .active').removeClass('active');
        jQuery('*[data-slide-to="' + index + '"]').addClass('active');
        if (!jQuery(this).hasClass('huffington')) {
            googletag.pubads().refresh();
        }
        ga('send', 'pageview');
        var urlref = location.href;
        PARSELY.beacon.trackPageView({
            url: urlref,
            urlref: urlref,
            js: 1,
            action_name: "Next Slide"
        });
        return true;
    });
    if (jQuery('.item.active')) {
        jQuery('.slide').find('iframe').each(function () {
            var url = jQuery(this).attr('src');
            jQuery(this).attr('data-src', url);
        });
    } else {
        jQuery('.slide').find('iframe').each(function () {
            var url = jQuery(this).attr('src');
            jQuery(this).attr('data-src', url);
        });
    }

    jQuery('.thumbs').click(function () {
        var img_src = jQuery(this).attr('src');
        jQuery(this).closest('.jstep').find('img.main').attr('src', img_src);
    });

    jQuery('.modal').on('show', function () {
        // Check to see if we have shown the video. If so, bail so that we don't embed multiples.
        if (jQuery(this).attr('data-shown') !== 'true') {
            // Get the URL of the video.
            var id = jQuery(this).attr('data-video');
            // Create a link in the modal body.
            jQuery('.modal-body .link', this).append('<a href="' + id + '" class="oembed">Video Link</a>');
            // Trigger the jQuery Oembed
            jQuery("a.oembed").oembed();
        }
    });

    jQuery('.modal').on('hide', function () {
        // Add a data-shown="true" to the modal. Will prevent further lookups.
        jQuery(this).attr('data-shown', 'true');
        // Look for the iframe tag, and clear the video SRC out.
        var video = jQuery('.modal-body', this).find('iframe');
        var url = video.attr('src');
        // Empty the src attribute so we can stop the video when it closes. Then we'll put it back right after.
        video.attr('src', '');
        video.attr('src', url);
    });
    jQuery('.huff .starter').click(function () {
        jQuery('.huff').removeClass('small');
        jQuery(this).hide();
        jQuery('.nexus').show();

        // Open external links inside gallery into new window
        jQuery('.scroller a').each(function () {
            var link = new RegExp('/' + window.location.host + '/');
            if (!link.test(this.href)) {
                jQuery(this).click(function (e) {
                    e.preventDefault();
                    e.stopPropagation();
                    window.open(this.href, '_blank');
                });
            }
        });

        // Listen for a keydown event and run the proper action.
        jQuery(document).keydown(function (event) {
            switch (event.which) {
                case 37:
                    jQuery('.carousel').carousel('prev');
                    jQuery('.carousel').on('slide', function () {
                        jQuery(this).carousel('pause');
                    });
                    break;
                case 39:
                    jQuery('.carousel').carousel('next');
                    jQuery('.carousel').on('slide', function () {
                        jQuery(this).carousel('pause');
                    });
                    break;
                case 27:
                    jQuery('.huff').addClass('small');
                    jQuery('.huff .starter').show();

                    // Disable our event listener
                    jQuery(document).off('keydown');
                    break;
            }
        });

    });
    jQuery(".huff .close").click(function () {
        jQuery('.huff').addClass('small');
        jQuery('.huff .starter').show();

        // Disable our event listener
        jQuery(document).off('keydown');
    });
    (function ($) {
        $(document.body).on('post-load', function () {
            googletag.pubads().refresh();
            ga('send', 'pageview');
        });
    })(jQuery);

    (function ($) {
        $(document.body).on('post-load', function () {
            googletag.pubads().refresh();
            ga('send', 'pageview');
        });
    })(jQuery);

    jQuery('.print-page').on('click', function () {
        window.print();
    });


    var currentDataDiff = '';
    var checkerSelectDiff = 0;
    var checkerSelectDur = 0;
    var $filter_diff_item = $('.diff-item li span');
    var $filter_duratin_item = $('.duration-item li span');


    //add classes for projects filter apply
    $(document).on('touchstart click', '.all-lvl', function () {
        $filter_diff_item.removeClass('diff-selected filter_selected');
        if (checkerSelectDiff == 0) {
            $(this).addClass('diff-selected filter_selected');
            checkerSelectDiff = 1;
        } else {
            $(this).removeClass('filter_selected diff-selected');
            checkerSelectDiff = 0;
        }
        $('.clear').addClass('show-me');
    });

    $(document).on('touchstart click', '.moderate', function () {
        $filter_diff_item.removeClass('diff-selected filter_selected');
        $('.all-lvl, .moderate').addClass('diff-selected');
        $(this).addClass('filter_selected');
        $('.clear').addClass('show-me');
        checkerSelectDiff = 0;
    });


    $(document).on('touchstart click', '.spec-skill', function () {
        $filter_diff_item.removeClass('diff-selected filter_selected');
        $('.spec-skill, .all-lvl, .moderate').addClass('diff-selected');
        $(this).addClass('filter_selected');
        $('.clear').addClass('show-me');
        checkerSelectDiff = 0;
    });

    $(document).on('touchstart click', '.1-3h', function () {
        $filter_duratin_item.removeClass('dur-selected filter_selected');
        if (checkerSelectDur == 0) {
            $(this).addClass('dur-selected filter_selected');
            checkerSelectDur = 1;
        } else {
            $(this).removeClass('filter_selected dur-selected');
            checkerSelectDur = 0;
        }
        $('.clear').addClass('show-me');
    });

    $(document).on('touchstart click', '.3-8h', function () {
        $filter_duratin_item.removeClass('dur-selected filter_selected');
        $('.3-8h').addClass('dur-selected');
        $('.1-3h').addClass('dur-selected');
        $(this).addClass('filter_selected');
        $('.clear').addClass('show-me');
        checkerSelectDur = 0;
    });

    $(document).on('touchstart click', '.8-16h', function () {
        $filter_duratin_item.removeClass('dur-selected filter_selected');
        $('.8-16h').addClass('dur-selected');
        $('.1-3h').addClass('dur-selected');
        $('.3-8h').addClass('dur-selected');
        $(this).addClass('filter_selected');
        $('.clear').addClass('show-me');
        checkerSelectDur = 0;
    });

    $(document).on('touchstart click', '.16h', function () {
        $filter_duratin_item.removeClass('dur-selected filter_selected');
        $('.8-16h').addClass('dur-selected');
        $('.1-3h').addClass('dur-selected');
        $('.3-8h').addClass('dur-selected');
        $('.16h').addClass('dur-selected');
        $(this).addClass('filter_selected');
        $('.clear').addClass('show-me');
        checkerSelectDur = 0;
    });

    //clear all filters
    $(document).on('touchstart click', '.clear', function (event) {
        event.stopPropagation();
        event.preventDefault();
        $filter_duratin_item.removeClass('dur-selected dur-hover filter_selected');
        $filter_diff_item.removeClass('diff-selected diff-hover filter_selected');
        $('div.post-filter p').removeClass('current');
        $('div.post-filter p.recent').addClass('current filter_selected');
        $('.clear').removeClass('show-me');
        $('.sortby div.post-filter p.recent').removeClass('filter_selected');
        $('.mobile-dur .duration-item li').removeClass('filter_selected').hide();
        $('.mobile_diff .diff-item li').removeClass('filter_selected current').hide();
        $('.diff-item li.first-item').show().addClass('current');
        checkerSelectDiff = 0;
        checkerSelectDur = 0;
        $('.first-item').show().addClass('current');
        $('.filter-applied').hide();
        $('.filter_max .filter').find('.open-list').removeClass('open-list');
        $('.filter_max .filter').find('.chosen').removeClass('chosen');

    });

    $(document).on('touchstart click', '.recent', function () {
        $('.popular').removeClass('current filter_selected');
        $('.recent').addClass('current filter_selected');
    });

    $(document).on('touchstart click', '.popular', function () {
        $('.recent').removeClass('current filter_selected');
        $('.popular').addClass('current filter_selected');
    });


    //add hover effects to filter on PC
    if (($('.all-projects').hasClass('pc')) || ($('.projects-cat').hasClass('pc'))) {
        $('.all-lvl').hover(function () {
            $('.all-lvl').toggleClass('diff-hover');
        });
        $('.moderate').hover(function () {
            $('.all-lvl').toggleClass('diff-hover');
            $('.moderate').toggleClass('diff-hover');
        });
        $('.spec-skill').hover(function () {
            $('.spec-skill').toggleClass('diff-hover');
            $('.all-lvl').toggleClass('diff-hover');
            $('.moderate').toggleClass('diff-hover');
        });
        $('.1-3h').hover(function () {
            $('.1-3h').toggleClass('dur-hover');
        });
        $('.3-8h').hover(function () {
            $('.1-3h').toggleClass('dur-hover');
            $('.3-8h').toggleClass('dur-hover');
        });
        $('.8-16h').hover(function () {
            $('.8-16h').toggleClass('dur-hover');
            $('.1-3h').toggleClass('dur-hover');
            $('.3-8h').toggleClass('dur-hover');
        });
        $('.16h').hover(function () {
            $('.8-16h').toggleClass('dur-hover');
            $('.1-3h').toggleClass('dur-hover');
            $('.3-8h').toggleClass('dur-hover');
            $('.16h').toggleClass('dur-hover');
        });
    }


    $(document).on('touchstart click', '.mobile_diff ul.diff-item li', function () {
        $('.mobile_diff ul.diff-item li').removeClass('filter_selected');
        $(this).addClass('filter_selected');
    });

    $(document).on('touchstart click', '.mobile-dur .duration-item li', function () {
        $(this).addClass('filter_selected');
    });


    //minify filter panel on scroll
    var indikator = ".minify .fa-chevron-down";
    var $min_filter = $(".minify .fa-chevron-down");
    var $max_filter = $(".minify .fa-chevron-up");
    var cat_wrapp_indicator = 0;
    var scrollTop = $(window).scrollTop();
    $(window).scroll(function () {
        var $window = $(window).width();
    });
    if (scrollTop >= '530') {
        $('.minify').addClass('sticky');
        $(indikator).show();
        var $window = $(window).width();
        if (($window > '768') || ( $('.all-projects ').hasClass('tablet') ) || ( $('.projects-cat ').hasClass('tablet') )) {
            if (cat_wrapp_indicator == 0) {
                $('.minify .cat-list-wrapp').hide();
            }
            else {
                $('.minify .cat-list-wrapp').slideDown();
            }
        }
    } else {
        $('.minify').removeClass('sticky');
        $max_filter.hide();
        $min_filter.hide();
        $('.minify .cat-list-wrapp').show();
    }
    $(window).scroll(function () {
        var scrollTop = $(window).scrollTop();
        var $window = $(window).width();
        if (scrollTop >= '300') {
            $('.minify').addClass('sticky').slideDown();
            $(indikator).show();
            if (($window > '768') || ( $('.all-projects ').hasClass('tablet') ) || ( $('.projects-cat ').hasClass('tablet') )) {

                if (cat_wrapp_indicator == 0) {
                    $('.minify .cat-list-wrapp').hide();
                } else {
                    $('.minify .cat-list-wrapp').show();
                }
            }
        } else {
            $('.minify').removeClass('sticky').hide();
            $max_filter.hide();
            $min_filter.hide();
            $('.minify .cat-list-wrapp').show();
        }
    });

    $min_filter.hide();
    $max_filter.click(function () {
        $('.minify .cat-list-wrapp').slideToggle("slow");
        $max_filter.hide();
        $min_filter.show();
        indikator = ".minify .fa-chevron-down";
        cat_wrapp_indicator = 0;
    });
    $min_filter.click(function () {
        $('.minify .cat-list-wrapp').slideToggle("slow");
        $min_filter.hide();
        $max_filter.show();
        indikator = ".minify .fa-chevron-up";
        cat_wrapp_indicator = 1;
    });

    //add adds show row
    $.ajax({
        url: '/ajax_get_shopify_featured_products.php',
        data: {
            'action': 'make_shopify_featured_products'
        },
        success: function (data) {
            $(".before-ads").after(data);
        },
        error: function (errorThrown) {
        }
    });


    //apply filter part
    $(document).on('touchstart click', '.filter-item ul li ul li span', function () {
        if ($(this).hasClass('clicks')) {
            $('.spinner').show();
            getProjects();
        }
    });
    $(document).on('touchstart click', '.clear', function () {
        $('.spinner').show();
        $('#mobile_cat').prop("selectedIndex", 0);
        $('#mobile_diff').prop("selectedIndex", 0);
        $('#mobile_dur').prop("selectedIndex", 0);
        getProjects();
    });
    $(document).on('touchstart click', '.post-filter p', function () {
        $('.spinner').show();
        getProjects();
    });

    $(document).on('touchstart click', '.mobile_diff .clicks', function () {
        $('.spinner').show();
        getProjects();
    });

    $(document).on('touchstart click', '.mobile-dur ul li', function () {
        if ($(this).hasClass('clicks')) {
            $('.spinner').show();
            getProjects();
        }
    });
    var paged = 1;

    function getProjects(type, callback) {
        var DataDiff, DataDur, DataCat, DataSort;
        var $window = $(window).width() + 17;
        if (typeof type === 'undefined') type = 'initial_load';
        if (typeof callback === 'undefined') callback = function () {
        };

        if (type === 'initial_load') paged = 1;
        if ($window <= 767) {
            DataDiff = $('#mobile_diff').find(":selected").attr('data-value');
            DataDur = $('#mobile_dur').find(":selected").attr('data-value');
            DataCat = $('#mobile_cat').find(":selected").attr('value');
        } else {
            DataDiff = $('.project-navigation ul.diff-item .filter_selected').attr('data-value');
            DataDur = $('.project-navigation ul.duration-item .filter_selected').attr('data-value');
            DataSort = $('.project-navigation .post-filter .filter_selected').attr('data-value');
            DataCat = $('.cat-list-wrapp h1').attr('data-value');
        }


        $.ajax({
            type: 'POST',
            url: '/wp-admin/admin-ajax.php',
            data: {
                action: 'sorting_posts',
                diff: DataDiff,
                dur: DataDur,
                sort: DataSort,
                cat: DataCat,
                type: type,
                paged: paged
            },
            success: function (data) {
                var projectsWrapper = $(".selected-posts-list");
                $('.spinner').hide();
                if (type === 'load_more') {
                    projectsWrapper.append(data);
                    callback();

                } else {
                    projectsWrapper.remove();
                    $(".posts-list").html(data);
                }

                var error_message = $(".posts-list").find('.error_message');
                if (error_message.length > 0) {
                    return;
                }

                $.ajax({
                    url: '/ajax_get_shopify_featured_products.php',
                    data: {
                        'action': 'make_shopify_featured_products'
                    },
                    success: function (data) {
                        $(".before-ads").after(data);
                    },
                    error: function (errorThrown) {
                    }
                });

                // Load placeholder ads.
                make.gpt.loadDyn();

            },
            error: function (errorThrown) {
            }
        });
    }

    //load more posts button
    $(document).on('touchstart click', '#pbd-alp-load-posts a', function () {

        if (!$(this).hasClass('first-click')) {
            $(this).addClass('first-click');
            paged++;
            // Show that we're working.
            $(this).text('Loading');
            $(this).parent().addClass('loading');
            $(".before-ads:first").removeClass('before-ads');
            getProjects('load_more', function () {

                var max_num_pages = $(".selected-posts-list").attr('data-max_num_pages');
                if (parseInt(max_num_pages) === paged) {
                    $('#pbd-alp-load-posts').remove();
                    return;
                }
                $('#pbd-alp-load-posts a').text('More');
                $('#pbd-alp-load-posts a').removeClass('first-click');
                $('#pbd-alp-load-posts').removeClass('loading');
                // Load placeholder ads.
                make.gpt.loadDyn();
            });
            return false;
        }
    });

    // MOBILE NAVIGATION
    $(document).mouseup(function (e) {
        var container = $(".filter_max");
        if ((container.has(e.target).length === 0) && (container.hasClass('sort'))) {
            $('.filter_max .sortby').hide();
            $('.filter_max').removeClass('show-now sort');
            $('.filter_mini').show();
        }
    });
    $(document).on('touchstart', 'body', function (e) {
        var container = $(".filter_max");
        if ((container.has(e.target).length === 0) && (container.hasClass('sort'))) {
            $('.filter_max .sortby').hide();
            $('.filter_max').removeClass('show-now sort');
            $('.filter_mini').show();
        }
    });
    var $adminBar = $('#wpadminbar').height();
    var $filterHeight = $(window).height() - 345 - $adminBar;
    $('.filter_max .filter .resets').css('margin-top', $filterHeight);
    $(document).on('touchstart click', '.filter_mini .filter-button', function (event) {
        event.stopPropagation();
        event.preventDefault();
        $('.filter_mini').hide();
        $('.filter_max .filter').slideDown();
        $('.filter_max').addClass('show-now');
    });

    $(document).on('touchstart click', '.filter_mini .sort-button', function (event) {
        event.stopPropagation();
        event.preventDefault();
        $('.filter_mini').hide();
        $('.filter_max .sortby').show();
        $('.filter_max').addClass('show-now sort');
    });

    $(document).on('touchstart click', '.close-button', function (event) {
        event.stopPropagation();
        event.preventDefault();
        $('.filter_max .filter').slideUp();
        $('.filter_mini').show();
        window.setTimeout(function () {
            $('.filter_max').removeClass('show-now');
        }, 570);
    });
    $(document).on('touchstart click', '.apply-button', function (event) {
        event.stopPropagation();
        event.preventDefault();
        $('.spinner').show();
        $('.filter_max .filter').slideUp();
        $('.filter_mini').show();
        window.setTimeout(function () {
            $('.filter_max').removeClass('show-now');
        }, 570);
        $('.filter-applied').show();
        getProjects();
    });

    $(document).on('change', '#mobile_cat', function () {
        var removeNbsp = $(this).find(":selected").html().replace('&nbsp;', '');
        $(this).find(":selected").html(removeNbsp);
    });

    $(document).on('touchstart click', '.mobile-dur ul.duration-item li', function (event) {
        event.stopPropagation();
        event.preventDefault();
        if ($(this).hasClass('current')) {
            $('.mobile-dur ul.duration-item li').removeClass('chosen').addClass('clicks');
            onClickDurationList();
            $(this).removeClass('current').show().addClass('chosen');
        } else {
            onClickDurationCheck();
            $(this).show('fast').addClass('current').removeClass('clicks');
        }
    });

    function onClickDurationList() {
        $('.mobile-dur ul.duration-item li').show().removeClass('filter_selected');
        $('.mobile-dur ul.duration-item').addClass('open-list');
    }

    function onClickDurationCheck() {
        $('.mobile-dur ul.duration-item li').removeClass('current');
        $('.mobile-dur ul.duration-item li').hide();
        $(this).show('fast');
        $('.mobile-dur ul.duration-item').removeClass('open-list chosen');
    }


    $(document).on('touchstart click', '.mobile_diff ul.diff-item li', function (event) {
        event.stopPropagation();
        event.preventDefault();
        if ($(this).hasClass('current')) {
            $('.mobile_diff ul.diff-item li').removeClass('chosen').addClass('clicks');
            onClickDiffList();
            $(this).removeClass('current').show().addClass('chosen');
        } else {
            onClickDiffCheck();
            $(this).show('fast').addClass('current').removeClass('clicks');
        }
    });

    function onClickDiffList() {
        $('.mobile_diff ul.diff-item li').show().removeClass('filter_selected');
        $('.mobile_diff ul.diff-item').addClass('open-list');
    }

    function onClickDiffCheck() {
        $('.mobile_diff ul.diff-item li').removeClass('current');
        $('.mobile_diff ul.diff-item li').hide();
        $(this).show('fast');
        $('.mobile_diff ul.diff-item').removeClass('open-list chosen');
    }


    //blog//

    //load more posts

    function loadMorePosts() {
        var currentButton = $('#blog-load-posts');
        if (!currentButton.hasClass('first-click')) {
            currentButton.addClass('first-click');
            var get_offset = currentButton.attr('data-offset');
            $.ajax({
                type: 'POST',
                url: '/wp-admin/admin-ajax.php',
                data: {
                    action: 'blog_output_with_ajax',
                    offset: get_offset
                },
                success: function (data) {
                    $('#blog-load-posts').remove();
                    $('.container.all-stories .post-list').append('<li class="row page-break"><div class="post">' + data);
                    currentButton.removeClass('first-click');
                    removeNbsp();
                    if ($('p').is('#blog-load-posts') === false) {
                        $('#footer').removeClass('non-visible');
                    }
                    ga('send', 'pageview',
                        { 'page': location.pathname + location.search + location.hash }
                    );
                    if(window.navigator.userAgent.indexOf("Chrome") > 0){
                        $window = $(window).width() + 17;
                    }else {
                        $window = $(window).width() ;
                    }
                    if($window <= 991){
                        changeCardsSmallBreakpoints();
                    }
                    // Load mobile ads or refresh sidebar ad.
                    if ($(window).width() < 768) {
                        var $start = $('.page-break:not(.rendered)').eq(0);
                        make.gpt.injectAds($start.nextAll(), {
                            'markup': '<li class=\'row ad-row\'><div class=\'js-ad scroll-load\' data-size=\'[[300,250]]\' data-pos=\'"btf"\'></div></li>',
                            'skipCount': 2,
                            'max' : 3
                        });
                        $start.toggleClass('rendered', true);
                    } else {
                        make.gpt.refresh();
                    }
                },
                error: function (data) {

                }
            });
        }
    }

    //load more tag archive
    function loadMoreTag() {
        var currentTagButton = $('#tag-load-posts');
        if (!currentTagButton.hasClass('first-click')) {
            currentTagButton.addClass('first-click');
            var get_offset = currentTagButton.attr('data-offset');
            var current_tag = $(".all-stories.tags").attr('data-slug');
            $.ajax({
                type: 'POST',
                url: '/wp-admin/admin-ajax.php',
                data: {
                    action: 'tag_output_with_ajax',
                    offset: get_offset,
                    slug: current_tag
                },
                success: function (data) {
                    currentTagButton.remove();
                    $('.container.all-stories .post-list').append('<li class="row page-break"><div class="post">' + data);
                    currentTagButton.removeClass('first-click');
                    removeNbsp();
                    if ($('p').is('#tag-load-posts') === false) {
                        $('#footer').removeClass('non-visible');
                    }
                    ga('send', 'pageview',
                        { 'page': location.pathname + location.search + location.hash }
                    );
                    if(window.navigator.userAgent.indexOf("Chrome") > 0){
                        $window = $(window).width() + 17;
                    }else {
                        $window = $(window).width() ;
                    }
                    if($window <= 991){
                        changeCardsSmallBreakpoints();
                    }
                    // Load mobile ads or refresh sidebar ad.
                    if ($(window).width() < 768) {
                        var $start = $('.page-break:not(.rendered)').eq(0);
                        make.gpt.injectAds($start.nextAll(), {
                            'markup': '<li class=\'row ad-row\'><div class=\'js-ad scroll-load\' data-size=\'[[300,250]]\' data-pos=\'"btf"\'></div></li>',
                            'skipCount': 2,
                            'max' : 3
                        });
                        $start.toggleClass('rendered', true);
                    } else {
                        make.gpt.refresh();
                    }
                },
                error: function (data) {

                }
            });
        }
    }


    //*********************//
    //sticky ads blog page//
    //*******************//
    if ($('.container').hasClass('all-stories')) {

        // Load mobile ads.
        if ($(window).width() < 768) {
            make.gpt.injectAds($('.post-list .row'), {
                'markup' : '<li class=\'row ad-row\'><div class=\'js-ad scroll-load\' data-size=\'[[300,250]]\' data-pos=\'"btf"\'></div></li>',
                'skipCount': 2,
                'max' : 3,
                'renderIntial' : false
            });
        }

        $('#footer').addClass('non-visible');
        /**
         * declarate sticky elements
         */
        if (($('.all-stories').hasClass('tags')) && ($('p').is('#tag-load-posts') === false)) {
            $('#footer').removeClass('non-visible');
        }
        if ((!$('.all-stories').hasClass('tags')) && ($('p').is('#blog-load-posts') === false)) {
            $('#footer').removeClass('non-visible');
        }
        var $sticky_adds_block = $('.all-stories .ad-refresh .js-ad').eq(0);
        var ads_position = $sticky_adds_block.offset().top - 57;
        var contentHeight;
        var notSticky = 0;
        $(window).scroll(function () {
            if ($('.all-stories').hasClass('tags')){
                contentHeight = $('.all-post-wrapper').height();
                if (contentHeight < 1500){
                    notSticky = 1;
                }
            }

            scrollTop = $(window).scrollTop();
            if ($('p').is('#blog-load-posts')) {
                var blogMorePosts = $('#blog-load-posts').offset().top - 2400;
            } else if ($('p').is('#tag-load-posts')) {
                var tagMorePosts = $('#tag-load-posts').offset().top - 2400;
            }
            if (scrollTop > blogMorePosts) {
                loadMorePosts();
            }
            if (scrollTop > tagMorePosts) {
                loadMoreTag();
            }
            $window = $(window).width() + 17;
            var footerMargin;
            if ($window > 767) {
                if ($window < 992) {
                    footerMargin = 58;
                }else{
                    footerMargin = 78;
                }
                var footer_top_height = $('#footer').offset().top - 678;
                var finish_ads_position = $('#footer').height() + 57 + footerMargin;
                if ((!$sticky_adds_block.hasClass('add_sticky')) && (!$sticky_adds_block.hasClass('add_static_position'))) {
                    ads_position = $sticky_adds_block.offset().top - 57;
                }
                /**
                 * if ads is footer sticky
                 */
                if (notSticky == 0){
                    if (scrollTop > footer_top_height) {
                        $sticky_adds_block.removeClass('add_sticky').addClass('add_static_position').css('bottom', finish_ads_position);
                    } else {
                        $sticky_adds_block.removeClass('add_static_position').css('bottom', '');
                    }
                    if ((scrollTop > ads_position) && (scrollTop < footer_top_height)) {
                        $sticky_adds_block.removeClass('add_static_position').addClass('add_sticky').css('bottom', '');
                    } else {
                        $sticky_adds_block.removeClass('add_sticky');
                    }
                }
            }
        });

        var newTitle;

        function removeNbsp() {
            $('.about-post h2 a').each(function () {
                newTitle = $(this).html().replace('&nbsp;', ' ');
                $(this).html(newTitle);
            });
            $('.posts-feeds-wrapper .p-title').each(function () {
                newTitle = $(this).html().replace('&nbsp;', ' ');
                $(this).html(newTitle);
            });
        }
        removeNbsp();
        var largeCard,index,newIndex;
        function changeCardsSmallBreakpoints(){
            $('.post-list li.row .post').each(function(){
                largeCard = $(this).find('.large-card').remove();
                $(this).prepend(largeCard);
            });
        }
        function changeCardsLargeBreakpoints(){
            newIndex = 3;
            $('.post-list li.row').each(function(i){
                index = i + 1;
                if ((index%2 == 0) && (index%4 != 0)){
                    largeCard = $(this).find('.large-card').remove();
                    $(this).find('.small-card').eq(0).after(largeCard);
                }
                if ((index == newIndex)){
                    largeCard = $(this).find('.large-card').remove();
                    $(this).find('.post').append(largeCard);
                    newIndex = newIndex + 4;
                }
            });
        }
        if(window.navigator.userAgent.indexOf("Chrome") > 0){
            $window = $(window).width() + 17;
        }else {
            $window = $(window).width() ;
        }
        if($window <= 991){
            changeCardsSmallBreakpoints();
        }
        $(window).resize(function () {
            if(window.navigator.userAgent.indexOf("Chrome") > 0){
                $window = $(window).width() + 17;
            }else {
                $window = $(window).width() ;
            }
            if ($window <= 991) {
                changeCardsSmallBreakpoints();
            }else{
                changeCardsLargeBreakpoints();
            }
        });
    }

});